#ifndef  __MS_d245__h																									
#define  __MS_d245__h
#include "stdint.h"
#include "rtc.h"

typedef struct
{
   unsigned char  P_Number1st;     /*��һ��Ʒ����ķ���*/
	 unsigned char  P_Number2nd;     /*�ڶ���Ʒ����ķ���*/
	 unsigned char  P_Number3rd;     /*������Ʒ����ķ���*/
	 unsigned char  P_Number4th;     /*���Ĳ�Ʒ����ķ���*/
   unsigned char  P_Cost1st;       /*��һ�ݹ���Ĳ�Ʒ���ܼ�*/
	 unsigned char  P_Cost2nd;       /*�ڶ��ݹ���Ĳ�Ʒ���ܼ�*/
	 unsigned char  P_Cost3rd;       /*�����ݹ���Ĳ�Ʒ���ܼ�*/
	 unsigned char  P_Cost4th;       /*���ķݹ���Ĳ�Ʒ���ܼ�*/
   unsigned char  P_paymoney;      /*���˶���Ǯ*/
   unsigned char  P_PayShould ;    /*�ø�����Ǯ*/
   unsigned	char  P_MoneyBack ;    /*�һض���Ǯ���ͻ�*/
   unsigned char  P_PayForCards ;  /*�����˶���Ǯ*/
   unsigned char  P_MealPrice ;	   /*���۶���Ǯ*/
   unsigned char  P_PayForBills ;  /*ֽ�Ҹ��˶���Ǯ*/
   unsigned char  P_PayForCoins ;  /*Ӳ�Ҹ��˶���Ǯ*/
}P_stuction;

extern P_stuction Print_Struct;

void PrintInit(void);
uint8_t CheckPrintStatus(void);
uint8_t PrintMessage(uint8_t flag);
void  SPRT(void);
void COPY(Struct_TD  a,unsigned char *p0,unsigned char *p1);

#endif	


