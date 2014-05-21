#include "rtc.h"
#include "stdio.h"
#include "printer.h"
#include "delay.h"
Struct_TD TimeDate ;
static RTC_TimeTypeDef RTC_TimeStructure;
static RTC_DateTypeDef RTC_DateStructure;	// RTC_DateStruct
static RTC_InitTypeDef RTC_InitStructure;
static RTC_AlarmTypeDef  RTC_AlarmStructure;
static __IO uint32_t AsynchPrediv=0,SynchPrediv=0;

u8 MyRTC_Init(void)
{
  	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)//δ����ʱ��
  	{  
    	RTC_Config();
    	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;//RTC �첽���� ��<0X7F��
    	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;//RTC ͬ������ ��<0X7FFF��
    	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24Сʱ��
    	if(RTC_Init(&RTC_InitStructure) == ERROR)return 1;
    	RTC_TimeRegulate();//����ʱ�� 
  	}
  	else//������ʱ��
  	{   
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	    PWR_BackupAccessCmd(ENABLE);//ʹ��RTC����
	    RTC_WaitForSynchro();//�ȴ�RTC APB�Ĵ���ͬ��
	    RTC_ClearFlag(RTC_FLAG_ALRAF);//���RTC���ӱ�־
//	    EXTI_ClearITPendingBit(EXTI_Line17);//����ж���17��־���ڲ�������RTC���ӣ�
//		printf("RTC_TimeStructure.RTC_Hours=%d\r\n",RTC_TimeStructure.RTC_Hours);
//   	printf("RTC_TimeStructure.RTC_Minutes=%d\r\n",RTC_TimeStructure.RTC_Minutes);
//    	printf("RTC_TimeStructure.RTC_Seconds=%d\r\n",RTC_TimeStructure.RTC_Seconds) ;
  	}
	return 0;
}
  /*******************************************************************************
 * ��������:RTC_GetTime_HM  �õ�ʱ��                                                               
 * ��    ��:void                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                  
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/
void RTC_Config(void)
{
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);//ʹ��RTC����
  	RCC_LSEConfig(RCC_LSE_ON);//ʹ���ⲿ����
  	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//�ȴ��ⲿ����׼����
  	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��RTCʱ��Դ  
  	SynchPrediv=0xFF;
  	AsynchPrediv=0x7F; 
  	RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��
  	RTC_WaitForSynchro();//�ȴ�RTC APB�Ĵ���ͬ��
//	RTC_WakeUpCmd(ENABLE );
}

  /*******************************************************************************
 * ��������:RTC_GetTime_HM  �õ�ʱ��                                                               
 * ��    ��:void                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                  
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/
void RTC_GetTime_HM(uint16_t *time)
{
 	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//	Hour   =RTC_TimeStructure.RTC_Hours;
//	Min    =RTC_TimeStructure.RTC_Minutes;

   *time=RTC_TimeStructure.RTC_Hours<<8 | RTC_TimeStructure.RTC_Minutes;

	printf("Time_Save\r\n");
}

  /*******************************************************************************
 * ��������:RTC_TimeShow  ʱ����ʾ                                                               
 * ��    ��:void                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                  
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/
void RTC_TimeShow()
{
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	TimeDate.Hours    =RTC_TimeStructure.RTC_Hours;
	TimeDate.Minutes  =RTC_TimeStructure.RTC_Minutes;
	TimeDate.Senconds =RTC_TimeStructure.RTC_Seconds;
	TimeDate.WeekDay  =RTC_DateStructure.RTC_WeekDay;
	TimeDate.Date     =RTC_DateStructure.RTC_Date;
	TimeDate.Month    =RTC_DateStructure.RTC_Month;
	TimeDate.Year     =RTC_DateStructure.RTC_Year;
}




  /*******************************************************************************
 * ��������:RTC_TimeRegulate  ����ʱ��                                                                
 * ��    ��:void                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                  
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/
 
  u32 tmp_hh = 0x17, tmp_mm = 0x3b, tmp_ss = 0x01;
  u32 tmp_yy = 0x09, tmp_m2 = 0x01, tmp_dd = 0x01,tmp_ww=0x01;
void RTC_TimeRegulate(void)
{
	  //printf("Set time\r\n");
	  RTC_TimeStructure.RTC_H12= RTC_HourFormat_24;		
   	RTC_TimeStructure.RTC_Hours = tmp_hh;
    RTC_TimeStructure.RTC_Minutes = tmp_mm;
    RTC_TimeStructure.RTC_Seconds = tmp_ss;	
  	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure)!=ERROR)
  	RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2);
		
  	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);//����RTC���ӼĴ���
  	RTC_ITConfig(RTC_IT_ALRA, ENABLE);//ʹ������A���ж�
  	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);//ʹ������A
	/***********************************
	һ��������������
	**************************************/		  
   RTC_DateStructure.RTC_WeekDay = tmp_ww;          // RTC_Weekday_Monday;
   RTC_DateStructure.RTC_Date = tmp_dd;
   RTC_DateStructure.RTC_Month =  tmp_m2;           //RTC_Month_January;
   RTC_DateStructure.RTC_Year = tmp_yy;
   if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure)!=ERROR)
   RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2);	  	    
   RTC_DateStructInit(&RTC_DateStructure);	  //
}

void RTC_AlarmShow()
{
  	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
}


