#define KEYTEXT
//#include "key.h"
//#include "Main.h"
//#include "ucos_ii.h"
//#include "app_cfg.h"
#include "Includes.h"

void InitKeyProc(void)
{
	//��ʼ����ⰴ������
	InitKeyScanData();
	InitKeyData();
}
//��ʼ�������������
void InitKeyScanData(void)
{
	u8 i=0;
	u8* pK=(u8*)&KeyPress;
	for(i=0;i<sizeof(KEY_SCAN);i++)	
		*(pK+i)=0;
}
	
void InitKeyData(void)
{
  KEY_TYPE* pKey=&KeyData;
	u8 *pTemp=(u8*)pKey;
	u8 i=0;
	for(i=0;i<sizeof(KEY_TYPE);i++)
		*(pTemp+i)=0;
}

u16 GetPressed(void)
{
	u16 temp=0;
	temp=GPIO_ReadInputData(GPIOC);
	temp &= KEY_NONE_PRESSED;
  return temp;
}

void ScanKey(void)
{
	switch(KeyPress.State){
		case KEY_IDLE_STATE:{//��������ȥ����
			if( KEY_NONE_PRESSED!=GetPressed() ){//�а�������
				KeyPress.State=KEY_TEST_DOWN_STATE;//��������ȥ����״̬
				KeyPress.Times=0;
				KeyPress.TempType=GetPressed();
			}
		}
		break;
		case KEY_TEST_DOWN_STATE:{//��ⰴ�����¶���
			if( KeyPress.TempType!=GetPressed() ){//֤���Ƕ���
				KeyPress.TempType=0;
				KeyPress.State=KEY_IDLE_STATE;
			}else//�Ƕ���
				KeyPress.State=KEY_LONG_TEST_STATE;//��ⳤ�̰�״̬
		}
		break;
		case KEY_LONG_TEST_STATE:{//��ⰴ���Ƿ�Ϊ����״̬
			if(GetPressed()==KeyPress.TempType){
				(KeyPress.Times)++;
				if(KeyPress.Times>=200){
					KeyPress.Times=0;
					KeyPress.State=KEY_LONG_TRUE_STATE;//���볤����̧�𶶶����״̬
				}
			}else{//û��2s��ֻҪ���жϣ���ʶ��Ϊ�̰�
				KeyPress.State=KEY_UP_STATE;
				KeyPress.Times=0;
			}
		}
		break;
		case KEY_LONG_TRUE_STATE:{//����ȷ��
			if(KeyPress.TempType == KEY_ENTR_PRESS)//ADDED 11.30
				KeyPress.Type = KEY_FAC_PRESS;//ADDED 11.30
			else if(KeyPress.TempType == KEY_ESC_PRESS)//ADDED 11.30
				KeyPress.Type = KEY_SET_PRESS;//ADDED 11.30
			KeyPress.Times=0;
			SysEvent.BitFlag.Key=TRUE;//���������¼�
			KeyPress.State=KEY_LONG_UP_STATE;			
		}
		break;
		case KEY_LONG_UP_STATE://����������
		if(GetPressed()!=KeyPress.TempType){
			(KeyPress.Times)++;
			if(KeyPress.Times>=2){
				KeyPress.State=KEY_IDLE_STATE;
				KeyPress.TempType=0;
				KeyPress.Times=0;
			}
		}else KeyPress.Times=0;
		break;
		case KEY_UP_STATE:{//�̰�ȥ����ȷ��  ������������Ѿ���10ms������
			if(GetPressed()!=KeyPress.TempType)
				KeyPress.State=KEY_TRUE_STATE;
		}
		break;
		case KEY_TRUE_STATE:{//�̰�ȷ��
			KeyPress.Type=KeyPress.TempType;//����Typeֵ�����������ù���ԭ
			KeyPress.TempType=0;
			SysEvent.BitFlag.Key=TRUE;//���������¼�
			KeyPress.Times=0;
			KeyPress.State=KEY_IDLE_STATE;						
		}
		break;
		default:
		break;
	}
}
	
void VerifyKeyType(void)
{
	switch(KeyPress.Type){
		case KEY_ESC_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_ESC;//���������ס���µİ������࣬�����Ĵ����л����´μ��ʱɾ���ñ�־
		}
		break;
		case KEY_UP_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_UP;
		}
		break;
		case KEY_DOWN_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_DOWN;
		}
		break;
		case KEY_ENTR_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_ENTR;
		}
		break;
		case KEY_SET_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_SET;
		}
		break;
		case KEY_FAC_PRESS:{
			KeyPress.Type=0;
			KeyData.Type=KEY_FAC;			
		}
		break;
		default:
			KeyPress.Type=0;
			KeyData.Type=0;			
		break;
	}
}
extern void SystickInit(void);


void TaskDisp(void *p_arg)
{
	for (;;)
	{

		OSTimeDly(1);
	}
}
void TaskKey(void *p_arg)
{
	SystickInit();
	InitKeyProc();
	KeyMail=OSMboxCreate((void*)0);
	OSTaskCreate(RefreshMenuTask, (void *)0,&MenuTask[MENU_STK_SIZE-1], KEY_MENU_PRIO);	
	while(1){  
		ScanKey();
		OSTimeDly(2);//��ʱ
	  ScanKey();
		if(SysEvent.BitFlag.Key){ //����ȷ��
			SysEvent.BitFlag.Key=0;
			VerifyKeyType();
			OSMboxPost(KeyMail,(void*)&KeyData);
			InitKeyProc();//防止RefreshMenuTask阻塞
		}
		OSTimeDly(2);
	}
}
