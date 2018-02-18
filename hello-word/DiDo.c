/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "global_var.h"
#include "Macro.h"
#include "DiDo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ResetDO1()     GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define SetDO1()    	 GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define ResetDO2()     GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define SetDO2()    	 GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define ResetDO3()     GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define SetDO3()    	 GPIO_SetBits(GPIOC, GPIO_Pin_15)
#define ResetDO4()     GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define SetDO4()    	 GPIO_SetBits(GPIOB, GPIO_Pin_8)


#define DI_STATE      GPIOE->IDR
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
u8 backWorkMode1,backWorkMode2,backWorkMode3,backWorkMode4;
u8 ssflag=0;
u8 DINumberB1=0;
u8 DINumberB2=0;
u8 DINumberB3=0;
u8 DINumberB4=0;
u8 DINumberB5=0;
u8 DINumberB6=0;
u8 DINumberB7=0;
u8 DINumberK1=0;
u8 DINumberK2=0;
u8 DINumberK3=0;
u8 DINumberK4=0;
u8 DINumberK5=0;
u8 DINumberK6=0;
u8 DINumberK7=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



void DoAlarm(void){
	u16 *pWorkMode,*pTempDelayTime,*pTempDelayTimeRE,*pAlarmx,*pDelayTime,*pAlarmLimit;
	u16 *p;
	u8 *pAlarmFlag,*pOUTDO,i;
	u8 *pStateDO;
	
	if(!ssflag){
		ssflag=1;
		backWorkMode1=DOSet.DO1.WorkMode;
		backWorkMode2=DOSet.DO2.WorkMode;
		backWorkMode3=DOSet.DO3.WorkMode;
		backWorkMode4=DOSet.DO4.WorkMode;
	}
	if(backWorkMode1!=DOSet.DO1.WorkMode){
		ssflag=0;DOxx.State.DO1=0;DO1Flagx=1;
		DOxx.OUT.DO1=0;TempDelayTime.DO1=0;TempDelayTimeRE.DO1=0;AlarmFlag.DO1=0;
	}
	if(backWorkMode2!=DOSet.DO2.WorkMode){
		ssflag=0;DOxx.State.DO2=0;DO2Flagx=1;
		DOxx.OUT.DO2=0;TempDelayTime.DO2=0;TempDelayTimeRE.DO2=0;AlarmFlag.DO2=0;
	}
	if(backWorkMode3!=DOSet.DO3.WorkMode){
		ssflag=0;DOxx.State.DO3=0;DO3Flagx=1;
		DOxx.OUT.DO3=0;TempDelayTime.DO3=0;TempDelayTimeRE.DO3=0;AlarmFlag.DO3=0;
	}
	if(backWorkMode4!=DOSet.DO4.WorkMode){
		ssflag=0;DOxx.State.DO4=0;DO4Flagx=1;
		DOxx.OUT.DO4=0;TempDelayTime.DO4=0;TempDelayTimeRE.DO4=0;AlarmFlag.DO4=0;
	}
	
	pStateDO=&DOxx.State.DO1;
	p=&MData.FHz;
//	p=&MData.xxxx+1;
	pWorkMode=&DOSet.DO1.WorkMode;
	pAlarmx=&DOSet.DO1.Alarmx;
	pAlarmLimit=&DOSet.DO1.AlarmLimit;
	pDelayTime=&DOSet.DO1.DelayTime;
	pTempDelayTime=&TempDelayTime.DO1;
	pTempDelayTimeRE=&TempDelayTimeRE.DO1;
	pOUTDO=&DOxx.OUT.DO1;
	pAlarmFlag=&AlarmFlag.DO1;
	
	for(i=0;i<4;i++){
		if(*(pWorkMode+i*5)<9){
			*(pOUTDO+i)=0;
			*(pTempDelayTime+i)=0;
			*(pTempDelayTimeRE+i)=0;
			*(pAlarmFlag+i)=0;
			*(pStateDO+i)=0;
		}
		
		if(*(pWorkMode+i*5)==10){//低报警;高解除
  		if(*(p+*(pAlarmx+i*5)-1)<*(pAlarmLimit+i*5)){
  			if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;
  			if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
			  }
  		}
  		else{
  			if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
  			if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
					//return;
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}	
  		}
  	}
		if(*(pWorkMode+i*5)==11){//高报警;低解除
			if(*(p+*(pAlarmx+i*5)-1)>*(pAlarmLimit+i*5)){
				if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
				}
			}
			else{//高解除
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
				if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
					//	return;
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}
			}
		}//高报警;低解除
		if(*(pWorkMode+i*5)==12){//低报警;高不解除
			if(*(p+*(pAlarmx+i*5)-1)<*(pAlarmLimit+i*5)){
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
				}
			}
			else{
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
			}
		}//低报警;高不解除
		if(*(pWorkMode+i*5)==13){//高报警;低不解除
			if(*(p+*(pAlarmx+i*5)-1)>*(pAlarmLimit+i*5)){
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
				}
			}
			else{
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
			}
		}//高报警;低不解除
		if(*(pWorkMode+i*5)==14){//相电压低报警
			if(MData.Uan<*(pAlarmLimit+i*5)||MData.Ubn<*(pAlarmLimit+i*5)||
				MData.Ucn<*(pAlarmLimit+i*5)){
				if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;	
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
				}
			}
			else{//高解除
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
				if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
					//	return;
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}
			}
		}//相电压低报警
		if(*(pWorkMode+i*5)==15){//相电压高报警
			if(MData.Uan>*(pAlarmLimit+i*5)||MData.Ubn>*(pAlarmLimit+i*5)||
				MData.Ucn>*(pAlarmLimit+i*5)){
				if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;	
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
				}
			}
			else{//低解除
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
				if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
					//	return;
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}
			}
		}//相电压高报警
		if(*(pWorkMode+i*5)==16){//线电压低报警
			if(MData.Uab<*(pAlarmLimit+i*5)||MData.Ubc<*(pAlarmLimit+i*5)||
				MData.Uca<*(pAlarmLimit+i*5)){
				if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
						//return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
				}
			}
			else{//高解除
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
				if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
						//return;
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}
			}
		}//线电压低报警
		if(*(pWorkMode+i*5)==17){//线电压高报警
			if(MData.Uab>*(pAlarmLimit+i*5)||MData.Ubc>*(pAlarmLimit+i*5)||
				MData.Uca>*(pAlarmLimit+i*5)){
				if(*(pTempDelayTimeRE+i)!=0)
  				*(pTempDelayTimeRE+i)=0;
				if((++*(pTempDelayTime+i))>*(pDelayTime+i*5)){
					//	return;
					//报警输出
					*(pOUTDO+i)=1;
					*(pStateDO+i)=1;//debug
					*(pAlarmFlag+i)=1;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
				}
			}
			else{//低解除
				if(*(pTempDelayTime+i)!=0)
  				*(pTempDelayTime+i)=0;
				if((++*(pTempDelayTimeRE+i))>*(pDelayTime+i*5)){
					*(pOUTDO+i)=0;
					*(pTempDelayTime+i)=0;
					*(pTempDelayTimeRE+i)=0;
					*(pAlarmFlag+i)=0;
					*(pStateDO+i)=0;//debug
				}
			}
		}//线电压高报警
		if(*(pWorkMode+i*5)==18){//强制解除报警
			*(pOUTDO+i)=0;
			*(pStateDO+i)=0;//debug
			*(pAlarmFlag+i)=0;
		}
		if(*(pWorkMode+i*5)==19){//强制置报警
			*(pOUTDO+i)=1;
			*(pStateDO+i)=1;//debug
			*(pAlarmFlag+i)=1;
		}
	}//for
	if(CHKBIT(IEQ_SeriX,BIT4))
	{
		if(DOxx.OUT.DO1)SetDO1();else ResetDO1();
		if(DOxx.OUT.DO2)SetDO2();else ResetDO2();
	}
	if(CHKBIT(IEQ_SeriX,BIT5))
	{
		if(DOxx.OUT.DO3)SetDO3();else ResetDO3();
		if(DOxx.OUT.DO4)SetDO4();else ResetDO4();
	}
}

void DI_COMM(void){
	u16 temp=0;	
	
	/*if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) DIxx.State.DI1=1; else DIxx.State.DI1=0;
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)) DIxx.State.DI2=1; else DIxx.State.DI2=0;
	if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2))  DIxx.State.DI3=1; else DIxx.State.DI3=0;*/
	

		if(CHKBIT(IEQ_SeriX,BIT2))
		{
		if(!CHKBIT(DI_STATE,BIT6))
		{
			DINumberK7=0;
			DINumberB7++;
		}
		else 
		{
			DINumberB7=0;
			DINumberK7++;
		}
		if(DINumberB7==19)
		{
			DINumberB7=0;
			DIxx.State.DI7=1;
		}
		if(DINumberK7==19)
		{
			DINumberK7=0;
			DIxx.State.DI7=0;
		}
	
		if(!CHKBIT(DI_STATE,BIT5))
		{
			DINumberK6=0;
			DINumberB6++;
		}
		else 
		{
			DINumberB6=0;
			DINumberK6++;
		}
		if(DINumberB6==19)
		{
			DINumberB6=0;
			DIxx.State.DI6=1;
		}
		if(DINumberK6==19)
		{
			DINumberK6=0;
			DIxx.State.DI6=0;
		}
		if(!CHKBIT(DI_STATE,BIT4))
		{
			DINumberK5=0;
			DINumberB5++;
		}
		else 
		{
			DINumberB5=0;
			DINumberK5++;
		}
		if(DINumberB5==19)
		{
			DINumberB5=0;
			DIxx.State.DI5=1;
		}
		if(DINumberK5==19)
		{
			DINumberK5=0;
			DIxx.State.DI5=0;
		}
		if(!CHKBIT(DI_STATE,BIT3)) 
		{
			DINumberK4=0;
			DINumberB4++;
		}
		else 
		{
			DINumberB4=0;
			DINumberK4++;
		}
		if(DINumberB4==19)
		{
			DINumberB4=0;
			DIxx.State.DI4=1;
		}
		if(DINumberK4==19)
		{
			DINumberK4=0;
			DIxx.State.DI4=0;
		}
		}
		if(CHKBIT(IEQ_SeriX,BIT1))
		{
		if(!CHKBIT(DI_STATE,BIT2)) 
		{
			DINumberK3=0;
			DINumberB3++;
		}
		else 
		{
			DINumberB3=0;
			DINumberK3++;
		}
		if(DINumberB3==19)
		{
			DINumberB3=0;
			DIxx.State.DI3=1;
		}
		if(DINumberK3==19)
		{
			DINumberK3=0;
			DIxx.State.DI3=0;
		}
		if(!CHKBIT(DI_STATE,BIT1)) 
		{
			DINumberK2=0;
			DINumberB2++;
		}
		else 
		{
			DINumberB2=0;
			DINumberK2++;
		}
		if(DINumberB2==19)
		{
			DINumberB2=0;
			DIxx.State.DI2=1;
		}
		if(DINumberK2==19)
		{
			DINumberK2=0;
			DIxx.State.DI2=0;
		}
		if(!CHKBIT(DI_STATE,BIT0)) 
		{
			DINumberK1=0;
			DINumberB1++;
		}
		else 
		{
			DINumberB1=0;
			DINumberK1++;
		}
		if(DINumberB1==19)
		{
			DINumberB1=0;
			DIxx.State.DI1=1;
		}
		if(DINumberK1==19)
		{
			DINumberK1=0;
			DIxx.State.DI1=0;
		}
		}
							
	
	DICOMMtemp=0;
	if(CHKBIT(IEQ_SeriX,BIT2))
	{
		temp=DIxx.State.DI7;
		temp=temp<<6;
		DICOMMtemp|=temp;
		temp=DIxx.State.DI6;
		temp=temp<<5;
		DICOMMtemp|=temp;
		temp=DIxx.State.DI5;
		temp=temp<<4;
		DICOMMtemp|=temp;
		temp=DIxx.State.DI4;
		temp=temp<<3;
		DICOMMtemp|=temp;
	}
	if(CHKBIT(IEQ_SeriX,BIT1))
	{
		temp=DIxx.State.DI3;
		temp=temp<<2;
		DICOMMtemp|=temp;
		temp=DIxx.State.DI2;
		temp=temp<<1;
		DICOMMtemp|=temp;
		DICOMMtemp|=DIxx.State.DI1;
	}
	MData.DIstate=DICOMMtemp;
}

void DO_COMM(void){
	u8 temp=0;	

	DOCOMMtemp=0;
	if(CHKBIT(IEQ_SeriX,BIT5))
	{
		temp=DOxx.State.DO4;
		temp=temp<<3;
		DOCOMMtemp|=temp;
		temp=DOxx.State.DO3;
		temp=temp<<2;
		DOCOMMtemp|=temp;
	}
	if(CHKBIT(IEQ_SeriX,BIT4))
	{
		temp=DOxx.State.DO2;
		temp=temp<<1;
		DOCOMMtemp|=temp;
		DOCOMMtemp|=DOxx.State.DO1;
	}
	MData.DOstate=DOCOMMtemp;
}

void DoAction(void){
	
	if(DoActionFlag)
	{
		DoActionFlag=0;
		if(DoActionNum==0xff00){//闭合
			if(DOSet.DO1.WorkMode==9){
					DOxx.OUT.DO1=DOxx.State.DO1=1;
					DO1Flag=1;
			}
		}
		else if(DoActionNum==0x0000){//断开
			if(DOSet.DO1.WorkMode==9)
					DOxx.OUT.DO1=DOxx.State.DO1=0;
		}
	}
	if(DoActionFlag1)
	{
		DoActionFlag1=0;
		if(DoActionNum1==0xff00){//闭合
			if(DOSet.DO2.WorkMode==9){
					DOxx.OUT.DO2=DOxx.State.DO2=1;
					DO2Flag=1;
			}
		}
		else if(DoActionNum1==0x0000){//断开
			if(DOSet.DO2.WorkMode==9)
					DOxx.OUT.DO2=DOxx.State.DO2=0;
		}
	}
	if(DoActionFlag2)
	{
		DoActionFlag2=0;
		if(DoActionNum2==0xff00){//闭合
			if(DOSet.DO3.WorkMode==9){
					DOxx.OUT.DO3=DOxx.State.DO3=1;
					DO3Flag=1;
			}
		}
		else if(DoActionNum2==0x0000){//断开
			if(DOSet.DO3.WorkMode==9)
					DOxx.OUT.DO3=DOxx.State.DO3=0;
		}
	}
	if(DoActionFlag3)
	{
		DoActionFlag3=0;
		if(DoActionNum3==0xff00){//闭合
			if(DOSet.DO4.WorkMode==9){
					DOxx.OUT.DO4=DOxx.State.DO4=1;
					DO4Flag=1;
			}
		}
		else if(DoActionNum3==0x0000){//断开
			if(DOSet.DO4.WorkMode==9)
					DOxx.OUT.DO4=DOxx.State.DO4=0;
		}
	}
}
/******************* (C) COPYRIGHT 2008 APT *****END OF FILE****/		
