#define KEYTEXT
//#include "key.h"
//#include "Main.h"
//#include "ucos_ii.h"
//#include "app_cfg.h"
#include "Includes.h"

void InitKeyProc(void)
{
	//初始化检测按键数据
	InitKeyScanData();
	InitKeyData();
}
//初始化按键检测数据
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
		case KEY_IDLE_STATE:{//按键按下去抖动
			if( KEY_NONE_PRESSED!=GetPressed() ){//有按键按下
				KeyPress.State=KEY_TEST_DOWN_STATE;//按键按下去抖动状态
				KeyPress.Times=0;
				KeyPress.TempType=GetPressed();
			}
		}
		break;
		case KEY_TEST_DOWN_STATE:{//检测按键按下抖动
			if( KeyPress.TempType!=GetPressed() ){//证明是抖动
				KeyPress.TempType=0;
				KeyPress.State=KEY_IDLE_STATE;
			}else//非抖动
				KeyPress.State=KEY_LONG_TEST_STATE;//检测长短按状态
		}
		break;
		case KEY_LONG_TEST_STATE:{//检测按键是否为长按状态
			if(GetPressed()==KeyPress.TempType){
				(KeyPress.Times)++;
				if(KeyPress.Times>=200){
					KeyPress.Times=0;
					KeyPress.State=KEY_LONG_TRUE_STATE;//进入长按的抬起抖动检测状态
				}
			}else{//没过2s，只要有中断，就识别为短按
				KeyPress.State=KEY_UP_STATE;
				KeyPress.Times=0;
			}
		}
		break;
		case KEY_LONG_TRUE_STATE:{//长按确立
			if(KeyPress.TempType == KEY_ENTR_PRESS)//ADDED 11.30
				KeyPress.Type = KEY_FAC_PRESS;//ADDED 11.30
			else if(KeyPress.TempType == KEY_ESC_PRESS)//ADDED 11.30
				KeyPress.Type = KEY_SET_PRESS;//ADDED 11.30
			KeyPress.Times=0;
			SysEvent.BitFlag.Key=TRUE;//产生按键事件
			KeyPress.State=KEY_LONG_UP_STATE;			
		}
		break;
		case KEY_LONG_UP_STATE://检测结束长按
		if(GetPressed()!=KeyPress.TempType){
			(KeyPress.Times)++;
			if(KeyPress.Times>=2){
				KeyPress.State=KEY_IDLE_STATE;
				KeyPress.TempType=0;
				KeyPress.Times=0;
			}
		}else KeyPress.Times=0;
		break;
		case KEY_UP_STATE:{//短按去抖动确立  长按检测那里已经有10ms不等了
			if(GetPressed()!=KeyPress.TempType)
				KeyPress.State=KEY_TRUE_STATE;
		}
		break;
		case KEY_TRUE_STATE:{//短按确立
			KeyPress.Type=KeyPress.TempType;//具体Type值在主程序中用过后复原
			KeyPress.TempType=0;
			SysEvent.BitFlag.Key=TRUE;//产生按键事件
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
			KeyData.Type=KEY_ESC;//暂且这里记住按下的按键种类，后续的处理中会在下次检测时删除该标志
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
		OSTimeDly(2);//延时
	  ScanKey();
		if(SysEvent.BitFlag.Key){ //按键确立
			SysEvent.BitFlag.Key=0;
			VerifyKeyType();
			OSMboxPost(KeyMail,(void*)&KeyData);
			InitKeyProc();//闃叉RefreshMenuTask闃诲
		}
		OSTimeDly(2);
	}
}
