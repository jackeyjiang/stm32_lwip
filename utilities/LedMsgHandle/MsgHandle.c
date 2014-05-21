#include "stm32f4xx.h"
#include "MsgHandle.h"
#include "serialscreen.h"
#include "bsp.h"

MealAttribute DefineMeal[4];	//����4����ϵ
FloorAttribute FloorMeal[FloorMealNum], CurFloor;
UserSelection UserAct;
FloorMealMessage FloorMealMessageWriteToFlash;
CoinsTotoalMessage CoinsTotoalMessageWriteToFlash;
uint8_t  WaitTime=0;

 /*******************************************************************************
 * ��������:StatisticsTotal                                                                     
 * ��    ��:ͳ����Ŀ                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
 void StatisticsTotal(void)
 {	 
    uint8_t i,j;
		for(j = 0; j < 4; j++)	 //4�����ID��
		{
		    DefineMeal[j].MealCount = 0;
		    for(i = 0; i < 15; i++)
		    {
		      if(FloorMealMessageWriteToFlash.FloorMeal[i].MealID == j + 1)	 /*ǰ���Ǹ���ʾ����ж��flash�����ID ������Ľ��бȽ�*/
			    {
			      DefineMeal[j].MealCount      = DefineMeal[j].MealCount + FloorMealMessageWriteToFlash.FloorMeal[i].MealCount;
				    DefineMeal[j].Position[i][0] = FloorMealMessageWriteToFlash.FloorMeal[i].FCount;
				    DefineMeal[j].Position[i][1] = FloorMealMessageWriteToFlash.FloorMeal[i].SCount;
				    DefineMeal[j].Position[i][2] = FloorMealMessageWriteToFlash.FloorMeal[i].TCount;
			    }
		    }
		}
 }
 /*******************************************************************************
 * ��������:InitSetting                                                                   
 * ��    ��:�����õ���������                                                                
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                     
 *******************************************************************************/  
void InitSetting(void)
{
	uint8_t cnt_t=0,id_t=0;
  if(FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealID == CurFloor.MealID)
  {
		CurFloor.MealCount = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealCount;
		CurFloor.FCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FCount;
		CurFloor.SCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].SCount;
		CurFloor.TCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].TCount;
	}
	else
	{
		CurFloor.MealCount = 0;
		CurFloor.FCount    = 0;
		CurFloor.SCount    = 0;
		CurFloor.TCount    = 0;
	}

	VariableChage(row_1st,CurFloor.FCount);
	VariableChage(row_2nd,CurFloor.SCount);
	VariableChage(row_3rd,CurFloor.TCount);
	//֮���ڼ���Ų�����
//  for(cnt_t = 0; cnt_t < FloorMealNum; cnt_t++)  //���Ҳ�
//	{
//    if(FloorMealMessageWriteToFlash.FloorMeal[cnt_t].MealCount ==0)
//		{
//		   CurFloor.FloorNum = FloorMealNum;
//			 break;
//		}			
//	}
//	VariableChage(floor_num,CurFloor.FloorNum);
}
 /*******************************************************************************
 * ��������:MealArr                                                                     
 * ��    ��: ��Ʒ������                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
int MoneyBack = 0;
void MealArr(unsigned char index)
{
    int PayBill = 0;
	  switch(index)
	  {
      case 1 :	/*�����Ʒ��ID*/
			{				
 		    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x20;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 2 :	/*�����Ʒ��ID*/
			{
 		    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x23;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 3 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x26;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 4 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x24;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;	 
	    default : break ;
	  }	
/*------�����Ʒ������-----------------*/
		CustomerSel.MealNo  =  0x01; //ȡһ����Ʒ�ϴ�һ������
//	  switch(index)
//	  {
//			case 1:CustomerSel.MealNo  =  UserAct.MealCnt_1st_t;break;
//			case 2:CustomerSel.MealNo  =  UserAct.MealCnt_2nd_t;break;
//			case 3:CustomerSel.MealNo  =  UserAct.MealCnt_3rd_t;break;
//			case 4:CustomerSel.MealNo  =  UserAct.MealCnt_4th_t;break;
//	  }
		
		/*�����Ʒ������*/
    CustomerSel.PayType =  UserAct.PayType;  //	UserAct.PayType  ;
	 if(1)//if(CustomerSel.PayType == '1')	/*������ֽ���*/
	 { 
		//	printf("UserAct.MoneyBack=%d\r\n",UserAct.MoneyBack);
		 /*ʮ����ת����16*/
		 CustomerSel.Change[0]      =	     MoneyBack / 10000000000 %100;
		 CustomerSel.Change[0]      =         CustomerSel.Change[0]/10 *16 +CustomerSel.Change[0]%10 ;   
		 CustomerSel.Change[1]      =	     MoneyBack / 100000000 %100;
		 CustomerSel.Change[1]      =         CustomerSel.Change[1]/10 *16 +CustomerSel.Change[1]%10 ;                              
		 CustomerSel.Change[2]      =	     MoneyBack / 1000000 %100;
		 CustomerSel.Change[2]      =         CustomerSel.Change[2]/10 *16 +CustomerSel.Change[2]%10 ;
		 CustomerSel.Change[3]      =	     MoneyBack / 10000 %100;
		 CustomerSel.Change[3]      =         CustomerSel.Change[3]/10 *16 +CustomerSel.Change[3]%10 ;
		 CustomerSel.Change[4]      =	     MoneyBack / 100 %100;
		 CustomerSel.Change[4]      =         CustomerSel.Change[4]/10 *16 +CustomerSel.Change[4]%10 ;
		 CustomerSel.Change[5]      =	     MoneyBack % 100 ;
		 CustomerSel.Change[5]      =         CustomerSel.Change[5]/10 *16 +CustomerSel.Change[5]%10 ;
		 MoneyBack = 0 ;
	 }
		/*�����Ʒ��ʣ�����*/
		CustomerSel.RemainMealNum[0]  =	 (DefineMeal[index-1].MealCount>>8)&0xff;
		CustomerSel.RemainMealNum[1]  =	  DefineMeal[index-1].MealCount &0xff;
		/*���������*/
		CustomerSel.MealName	   =  	   index ;
		/*���˶����ֽ�*/
		//PayBill  =UserAct.PayForBills +*100 ;  /*����10��*/
		//PayBill  =	(UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards)*100 ;
	  switch(index)
		{
			case 1: PayBill= price_1st*100;break;
			case 2: PayBill= price_2nd*100;break;
			case 3: PayBill= price_3rd*100;break;
			case 4: PayBill= price_4th*100;break;
			default:break;
		}
	 	/*֧���˶���Ǯ*/
		CustomerSel.DealBalance[0]      =	      PayBill / 10000000000 %100;
		CustomerSel.DealBalance[0]      =       CustomerSel.DealBalance[0]/10 *16 +CustomerSel.DealBalance[0]%10 ;   
		CustomerSel.DealBalance[1]      =	      PayBill / 100000000 %100;
		CustomerSel.DealBalance[1]      =       CustomerSel.DealBalance[1]/10 *16 +CustomerSel.DealBalance[1]%10 ;                              
		CustomerSel.DealBalance[2]      =	      PayBill / 1000000 %100;
		CustomerSel.DealBalance[2]      =       CustomerSel.DealBalance[2]/10 *16 +CustomerSel.DealBalance[2]%10 ;
		CustomerSel.DealBalance[3]      =	      PayBill / 10000 %100;
		CustomerSel.DealBalance[3]      =       CustomerSel.DealBalance[3]/10 *16 +CustomerSel.DealBalance[3]%10 ;
		CustomerSel.DealBalance[4]      =	      PayBill / 100 %100;
		CustomerSel.DealBalance[4]      =       CustomerSel.DealBalance[4]/10 *16 +CustomerSel.DealBalance[4]%10 ;
		CustomerSel.DealBalance[5]      =	      PayBill % 100 ;
		CustomerSel.DealBalance[5]      =       CustomerSel.DealBalance[5]/10 *16 +CustomerSel.Change[5]%10 ;
}
