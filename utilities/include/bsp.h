#ifndef _bsp_H
#define _bsp_H

/*******��״̬***********/
#define current_temperature  0x01
#define waitfor_money        0x02
#define payment_success      0x03
#define hpper_out            0x04
#define print_tick           0x05
#define meal_out             0x06
#define data_upload          0x07
#define data_record          0x08
#define erro_hanle           0x09

/**********�ȴ���Ʒ״̬************/

#define takeing_meal   0x01 //����ȡ��
#define tookone_meal   0x02 //��ȡһ�ݲ�
#define tookkind_meal  0x03 //��ȡһ�ֲ�
#define takeafter_meal 0x04 //ȡ�����
#define takemeal_erro  0x05 //ȡ�ʹ���

/**********�쳣******************/
#define outage_erro      0x01 //�ϵ�
#define sdcard_erro      0x02 //SD���洢�쳣
#define billset_erro     0x03 //ֽ�һ��쳣
#define coinset_erro     0x04 //Ͷ�һ�
#define coinhooperset_erro   0x05 //�˱һ�
#define coinhooperset_empty  0x06 //�����ù�
#define printer_erro     0x07 //��ӡ���쳣
#define cardread_erro    0x08 //�������쳣
#define network_erro     0x09 //�����쳣
#define signin_erro      0x0a //ǩ���쳣
#define cardchck_erro    0x0b //����ͨ���п�ǩ���쳣



#define X_timeout        0x10 //x�ᴫ������ʱ
#define X_leftlimit      0x11 //��������������
#define X_rightlimit     0x12 //����Ҷ����������
#define mealtake_timeout 0x13 //ȡ�Ϳڴ�������ʱ
#define Y_timeout        0x14 //y�ᴫ������ʱ
#define link_timeout     0x15 //���ӳ�ʱ
#define Z_timeout        0x16 //z�ᴫ������ʱ
#define Z_uplimit        0x17 //z������϶�������
#define Z_downlimit      0x18 //z����¶�������
#define solenoid_timeout 0x19 //��ŷ���ʱ
#define Eeprom_erro      0x1a //eeprom �쳣
#define SendUR6Erro      0x1b  //���������쳣��ʱ
#define GetMealError     0x1c  //��е��5��ȡ������
#define MealNoAway       0x1d   //���ڳ��Ϳ�20�뻹δ��ȡ��

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "MsgHandle.h"	         //������ڵ�͵�����
#include "vioce.h"			       //��������
#include "serialscreen.h"	       //����������
#include "protocol.h"            //��е���ӳ���
//#include "led.h"			       //led�ƺ���
//#include "ICcard.h"			       //ic������
#include "timer2.h" 				 //���ڵ���ʱ
#include "timer4.h"			     //��ʱ��4 ������ʾ�¶ȵ� û���ٴη���һ���źŸ��ӻ��õ��¶ȡ�Ȼ������ʾ��
#include "timer3.h"		           //�������
#include "timer6.h"				   //���ڲɼ��¶�
#include "delay.h"			       //��ʱ
#include "uart2.h"           //����ͨ�������ӿ�
#include "uart3.h"				   //�������ӿ�
#include "uart4.h"           //ֽ�һ��ӿ�
#include "uart6.h"           //��е�ֽӿ�
#include "pvd.h"
#include "eeprom.h"
#include "intel_flash.h."

//#include "led.h"				   //led��
#include "coin.h"				     //Ӳ�һ�
#include "mini_hopper.h"     //�˱һ�
#include "stdio.h"				   //��׼�⺯��	 
#include "string.h"				   //��׼�⺯��	
#include "stdint.h"          //��������ͱ���
#include "bill.h"				     //ֽ�һ�
#include "printer.h"				 //��ӡ��
#include "uart1.h"				   //��ӡ���ӿ�
#include "ff.h"
#include "integer.h"
#include "diskio.h"
	
#include "rtc.h"
#include "sd.h"

#include "spi_flash.h"  
#include "szt_gpboc.h"       //����ͨ������

#include "network_handle.h"    //��̨�������ݵĴ���
#include "uart5.h"				     //�������ݴ���ӿ�

typedef enum
{
  Status_OK= 0 ,	   //���������ɹ� ���� 0
  Status_Error ,	  // ��������  ����1
	Status_Action 	  // ������ִ�� ���� 2 
} status ;




typedef struct 
{
  uint8_t   F_RX_Length ;   //�������ݳ��ȱ�־
  uint8_t   Response[6];     //ACK����
  uint8_t   F_RX1_SYNC;     //�Ƿ�ʼ������Ч����
  uint8_t   PacketData[1000]; //���ܵ�����
  uint8_t   PacketDCS;       //У����
  uint16_t  DataLength;    //���ݳ���
  uint16_t   RX_NUM ;	    //��λ
  uint8_t   Status   ;		 //״̬
  uint8_t   Checkoutright ;	  //����У���Ƿ�ɹ�
  uint8_t   ACKStatus;
}USART_StatusTypeDef;

extern  USART_StatusTypeDef  STATUS ;


extern uint8_t   exitflag;
extern uint8_t   CoinCountFlag;
extern uint8_t   TemperatureFlag  ;
extern uint8_t   BillActionFlag;      //��ʾֽ�һ�����Ǯ�С����ʱ���ܹر�ֽ�һ�
extern uint8_t   CurrentPoint ;
extern uint8_t   CoinFlag;			 //�����˱һ��ı�־
extern uint8_t   TemperatureCur;
extern uint8_t   Current ;    //��״̬
extern uint8_t   CurrentPointer; //��Ʒ����״̬
extern uint16_t  erro_flag;  //����״̬
extern int32_t   erro_record;  //������λ

bool CloseCashSystem(void);
bool OpenCashSystem(void);	
bool FindMeal(MealAttribute *DefineMeal); 
// uint8_t CanRecvCmd(uint8_t p);
unsigned char  WaitPayMoney(void);
uint8_t WaitMeal(void);
void hardfawreInit(void);
void  WaitTimeInit(uint8_t *Time);
void StateSend(void);
void DataUpload(char takemeal_flag);
void PrintTickFun(unsigned char *PrintTickFlag);
void PowerupAbnormalHandle(int32_t erro_record);
void AbnormalHandle(uint16_t erro);
void PollAbnormalHandle(void);
void SaveUserData(void);
void ReadUserData(void);
void AcountCopy(void);
void PayBackUserMoney(void);
// void LcdHandler(void);
// void  TempHandler(void);
// void ClearingFuntion(void) ;//��ǩ����
#endif
