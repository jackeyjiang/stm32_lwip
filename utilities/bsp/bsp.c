#include "stdint.h"
#include "bsp.h"
 /*******************************************************************************
 * ��������:WaitTimeInit                                                                     
 * ��    ��:��WaitTime���°��ŵ��µ�ֵ                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
 void  WaitTimeInit(uint8_t *Time)
 {
       	*Time = 60;
 }

 /*******************************************************************************
 * ��������:FindMeal                                                                     
 * ��    ��:�����Ǹ��ط��в�                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
 uint16_t   Balance;
 static     uint8_t IsCard	;
 uint8_t    DisplayTimes	 ;
 uint8_t    Line = 0,Column = 0 ;
 bool FindMeal(MealAttribute *DefineMeal) 
 {
    for(Line = 0; Line < FloorMealNum; Line++)  //���Ҳ�
		{
			for(Column = 0; Column < 3; Column++)     //������
			{
			  if(DefineMeal[UserAct.MealID - 1].Position[Line][Column] > 0)	
			    return true;		                           //����λ���в͵Ļ�����0
			}
		}
		return false ;
 }


  /*******************************************************************************
 * ��������:PrintTick                                                                     
 * ��    ��:�Ƿ��ӡСƱ                                                                
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
			
void PrintTickFun(unsigned char *PrintTickFlag)
{      
	if(*PrintTickFlag == 0x01 )
	{
		 TIM_Cmd(TIM4, DISABLE);
	 	*PrintTickFlag = 0 ;
		/*��ӡСƱ�ĺ���*/
		 SPRT();
	   TIM_Cmd(TIM4, ENABLE);
	}	 
	if(*PrintTickFlag == 0x02 )
	{
	   TIM_Cmd(TIM4, DISABLE);
		 *PrintTickFlag = 0;
		 TIM_Cmd(TIM4, ENABLE);
	}
	
}	
  /*******************************************************************************
 * ��������:WaitPayMoney                                                                     
 * ��    ��:�ȴ������û�Ͷ��Ǯ                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 

uint8_t CurrentPoint = 1 ; 
uint8_t UserPayMoney =0 ;
unsigned char NewCoinsCnt=0;
unsigned char OldCoinsCnt=0;
unsigned char  WaitPayMoney(void)
{
//	uint8_t temp = 0;
	uint8_t temp1;
	VariableChage(wait_payfor,WaitTime);
  switch(CurrentPoint)
	{
	  case 1 : 
	  {
	    /*��ʾ���ʽ���ֽ����п�������ͨ*/
			if(WaitTime<56)
			{
			  CurrentPoint = 3;
			  /*֧����ʽ*/			 
			  UserAct.PayType = '1';/* �ֽ�֧��*/
			}
		}break;    		
	  case 2:  //����Ļ������ת
	  {
	    /*����ѡ�񸶿ʽ����*/
			CurrentPoint = 3 ;
		  UserAct.PayType = '1' ;/* �ֽ�֧��*/
	  }break;	  
		         
	  case 3 :  //��Ǯ
		{			
	    UserPayMoney = ReadBills();			 
		  if(UserPayMoney !=0 )	   //��ʾ�յ���Ǯ
		  {
			  UserAct.PayAlready  += UserPayMoney;
			  UserAct.PayForBills += UserPayMoney;	
				VariableChage(payment_bill,UserAct.PayForBills);
			  UserPayMoney = 0 ;
		  }
			CurrentPoint = 5 ;
		}break;    					
	  case 5 ://��ʾ�����˶���Ӳ��	
	  {
		  VariableChage(payment_coin,UserAct.PayForCoins);	
		  VariableChage(payment_card,UserAct.PayForCards); 
			CurrentPoint = 6;
		}break;		    
	  case 6 : //ͳ��Ǯ��
    {
	    if(UserAct.PayAlready +UserAct.PayForCards>=UserAct.PayShould)		//Ͷ��Ǯ���ڵ���Ҫ����Ǯ
		  {     
		    CurrentPoint = 9;	             
	   	}
	    else
	    { 
		    CurrentPoint = 3; 
	    } 	
		}break;   				 
    case 7 :  /*���п�֧������Ļ����*/
		{
			WaitTimeInit(&WaitTime);
			PageChange(Cardbalence_interface);
			UserAct.PayType = '2' ;/* ���п�֧��*/
			temp1 = 0;
			temp1 = GpbocDeduct(UserAct.PayShould-UserAct.PayAlready); //*100;
			//temp1 = GpbocDeduct(1);
			if(temp1 == 1)
			{
				UserAct.PayForCards = UserAct.PayShould - UserAct.PayAlready;
			  //UserAct.PayForCards = UserAct.PayShould ;
			  UserAct.PayAlready += UserAct.PayForCards ;
				UART3_ClrRxBuf();
			  CurrentPoint =6;
			}
			else
			{
				WaitTimeInit(&WaitTime);
				PageChange(Acount_interface+2);
				CurrentPoint = 1;
			  /*֧����ʽ*/			 
			  UserAct.PayType = 0x00;/* �ֽ�֧��*/			
        UART3_ClrRxBuf();
			}
	  }break;
	  case 8 :/*����֧ͨ������Ļ����*/
	  {
			WaitTimeInit(&WaitTime);
			PageChange(Cardbalence_interface);
	    UserAct.PayType = '3' ;/* ����֧ͨ��*/
			temp1 = 0;
			temp1 = SztDeduct(UserAct.PayShould - UserAct.PayAlready); //*100;
			if(temp1 == 1)
			{
				UserAct.PayForCards = UserAct.PayShould - UserAct.PayAlready;
			  //UserAct.PayForCards = UserAct.PayShould ;
			  UserAct.PayAlready += UserAct.PayForCards ;
				UART3_ClrRxBuf();
			  CurrentPoint =6;
			}
			else
			{
				WaitTimeInit(&WaitTime);
				PageChange(Acount_interface+2);
				CurrentPoint = 1;
			  /*֧����ʽ*/			 
			  UserAct.PayType = 0x00;/* �ֽ�֧��*/			
        UART3_ClrRxBuf();
			}
		}break;
	  case 9 :  //����ɹ��ر����е�����ϵͳ
		{
			UserAct.MoneyBack = UserAct.PayAlready - UserAct.PayShould;	
			UserAct.Meal_totoal= UserAct.MealCnt_4th+UserAct.MealCnt_3rd+UserAct.MealCnt_2nd+UserAct.MealCnt_1st;
      OldCoinsCnt= UserAct.MoneyBack ; //���������ִֻ��һ��
			VariableChage(mealout_totle,UserAct.Meal_totoal);	
      NewCoinsCnt= 0; 
			CloseTIM7();
			CloseTIM3();
      //����Ҫ��ӡ�����ݸ�ֵ����ؽṹ��	
     	Print_Struct.P_Number1st = UserAct.MealCnt_1st;
			Print_Struct.P_Number2nd = UserAct.MealCnt_2nd;
			Print_Struct.P_Number3rd = UserAct.MealCnt_3rd;
			Print_Struct.P_Number4th = UserAct.MealCnt_4th;
			Print_Struct.P_Cost1st   = UserAct.MealCost_1st;
			Print_Struct.P_Cost2nd   = UserAct.MealCost_2nd;
			Print_Struct.P_Cost3rd   = UserAct.MealCost_3rd;
			Print_Struct.P_Cost4th   = UserAct.MealCost_4th;
			Print_Struct.P_paymoney  = UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards ;
			Print_Struct.P_PayShould = UserAct.PayShould ;
			Print_Struct.P_MoneyBack = UserAct.MoneyBack ;
			CurrentPoint = 0 ;
	       return  Status_OK;
		}
	  default :break;
  }
	if(WaitTime==0) 
	{
    WaitTimeInit(&WaitTime);
		PageChange(Menu_interface);//��ʱ�˳��û���Ʒ����ѡ�����
		if(!CloseCashSystem()) printf("cash system is erro6\r\n");  //�ر��ֽ����
		CurrentPoint = 0 ;
		UserAct.MoneyBack= UserAct.PayAlready; //��ʱ���յ���Ǯ��Ӳ�ҵ���ʽ����
		ClearUserBuffer();//����û�����
		if(UserAct.MoneyBack>0)
		Current= hpper_out;
	}
  return  Status_Action;
}												


uint8_t WaitMeal(void)
{
	uint8_t MealoutCurrentPointer=0;
  //static unsigned char Cmd[20]={0x05, 0x31, 0x30, 0x30, 0x31, 0x30 ,0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x34, 0x4D, 0x31, 0x35, 0x43, 0x03, 0x0D ,0x0A};
	uint8_t temp;
	do
	{
		switch(MealoutCurrentPointer)
		{
			case 0 : /*�����û���ѡ��Ʒ��λ��*/
			{
				//��ֵ��ǰUserAct.MealID
				if(UserAct.MealCnt_1st>0)
				{
					UserAct.MealID = 0x01; 
					goto loop3;
				}				
				else if(UserAct.MealCnt_2nd>0)
				{
					UserAct.MealID = 0x02;
					goto loop3;
				}				
				else if(UserAct.MealCnt_3rd>0)
				{
					 UserAct.MealID = 0x03;	
						goto loop3;
				}
				else if(UserAct.MealCnt_4th>0)
				{
					 UserAct.MealID = 0x04;	
						goto loop3;
				}
				else
				{	
					 //�����Ʒȫ�����꣬�˳���������		
					 //printf("takeafter_meal\r\n");		
					 PlayMusic(VOICE_12);				
					 return takeafter_meal;
				}
	loop3:	if(FindMeal(DefineMeal)) /*���Ҳ�ƷID��λ��*/
					 MealoutCurrentPointer= 1;
			}break;
			case 1 : /* ���Ͳ͵���Ŀ��һ*/
			{			 
				FloorMealMessageWriteToFlash.FloorMeal[Line].MealCount --; 
				switch(Column)
				{
					case 0 :   FloorMealMessageWriteToFlash.FloorMeal[Line].FCount -- ;	  break;
					case 1 :   FloorMealMessageWriteToFlash.FloorMeal[Line].SCount -- ;	  break;
					case 2 :   FloorMealMessageWriteToFlash.FloorMeal[Line].TCount -- ;	  break;
					default:  break;
				}
				DefineMeal[UserAct.MealID - 1].Position[Line][Column]--;
				DefineMeal[UserAct.MealID - 1].MealCount--;
				WriteMeal();
				StatisticsTotal();
				DispLeftMeal(); 
				MealoutCurrentPointer = 2 ;
			}break ;   
			case 2 : /*�����к��е�λ�ã��ȴ���Ӧ*/
			{
					//����[Line][Column]��ֵ�������� �ȴ�ACK	
				//printf("�����к��е�λ�ã��ȴ���Ӧ\r\n");
				temp =0;
				temp = OrderSendCoord(Line,Column);
				
				if( temp ==1)//���ͳɹ�
				{
					LinkTime =0;
					machinerec.rerelative =0;   //���Ǵ���λ��
					MealoutCurrentPointer = 3 ;
				}
				else				//����ʧ��
				{
					//printf("send coord error\r\n");
					//AbnormalHandle(SendUR6Erro);
					erro_record |= (1<<SendUR6Erro);
					return takemeal_erro;
				}
			}break;  
			case 3 :    /*����ȡ������*/
			{
				//printf("����ȡ������\r\n");
				//��ѯ��е���Ƿ�׼���ã����׼���÷���ȡ������
				//�����ʱ�� ���ش���
				//���û�в�Ʒ CurrentPointer=0;  else CurrentPointer=3
				while(1)  //��ѯ��е���Ƿ�׼���ã�
				{
					if(LinkTime > 10)    //��ʱ
					{
						LinkTime =0;
						//printf("move to coord timeout!\r\n");
						//AbnormalHandle(SendUR6Erro);
						erro_record |= (1<<SendUR6Erro);
						return takemeal_erro;
					}       
					manageusart6data();  //����е�������ݣ������е�ַ�������
					if(machinerec.regoal ==1)   //����ȡ�͵�
					{
						 break;
					}
				}
				if(machinerec.regoal ==1)   //����ȡ�͵�
				{
					machinerec.regoal =0 ;
					temp =0;
					temp = OrderGetMeal();   //����ȡ������
					LinkTime =0;
					MealoutCurrentPointer=4;  				
					if(temp ==1)       // ȡ������ͳɹ�
					{
						LinkTime =0;
						MealoutCurrentPointer=4;  
					}
					else          //����ʧ��     
					{
						//printf(" send getmeal order error\r\n");
						//AbnormalHandle(SendUR6Erro);
						erro_record |= (1<<SendUR6Erro);
						return takemeal_erro;
					}
				}
			}break;
			case 4 :  	/*������������ȡ��*/
			{
				//CurrentPointer=5; 
				//PlayMusic(VOICE_9);			
				//printf("������������ȡ��");			
				 //�����Ʒ����ȡ�Ϳڲ�������
				//�����Ʒȡ���� �����ӳ�����������ϴ�  
				while(1)
				 {
					 if(LinkTime >120)   //�ӷ���ȡ������󵽲��ѵ�����Ϳڣ����ȴ�һ����
					 {
						 printf("from send getmeal order to sell door timeout!\r\n");
						 //AbnormalHandle(SendUR6Erro);
						 erro_record |= (1<<SendUR6Erro);
						 return takemeal_erro;
						 break;
					 }
					 manageusart6data();   //����е�������ݣ������е�ַ�������
					 if(machinerec.retodoor == 1)   //������Ϳ�
					 {
						 machinerec.retodoor = 0;
						 //������ȡ������
							PlayMusic(VOICE_9);
						 break;
					 }
					 if(machinerec.reenablegetmeal ==1)  //ȡ��5���˻�δȡ����
					 {
						 machinerec.reenablegetmeal =0; //�¼ӵ���Ҫ����ر������
						 printf("ȡ��5���˻�δȡ����\r\n");	 
						 erro_record |= (1<<GetMealError);
						 //AbnormalHandle(GetMealError);
						 return takemeal_erro;
						 break;
					 }
				 }
				 if( machinerec.retodoor == 1) //������Ϳ�
				 {
					 machinerec.retodoor = 0;				 
				 }
				 LinkTime =0;
				 while(1)//�ȴ��ͻ�ȡ�߲�֮�󣬲�����Case 5
				 {
					 if( LinkTime >180) //���ڳ��Ϳ�δ��ȡ�ߣ����ȴ�3�֣������˱���
					 {
						 //printf("��δ�����������ӻ�δ��ȡ��\r\n");
						 //AbnormalHandle(MealNoAway);
						 erro_record |= (1<<MealNoAway);
						 return takemeal_erro;
					 }
				 manageusart6data();   //����е�������ݣ������е�ַ�������
				 if( machinerec.remealaway == 1) //���ѱ�ȡ��
				 {
						 //printf("���ѱ�ȡ��\r\n");
						 //machinerec.remealaway = 0;
						 LinkTime =0;
						 machinerec.remealaway = 0;
						 MealoutCurrentPointer=5;
						 break;
				 }
				 if( machinerec.remealnoaway == 1)  //����ȡ�Ϳڹ���20�뻹δ��ȡ��
				 {
						 //printf("����ȡ�Ϳڹ���20�뻹δ��ȡ��\r\n");
						 PlayMusic(VOICE_10);
						 //machinerec.remealnoaway = 0;
						 LinkTime =0;
						 machinerec.remealnoaway = 0;
						 MealoutCurrentPointer=5;
						break;
						 //������ʾ����ȡ�߳��ͿڵĲ� "
					} 
				}
				if( machinerec.remealaway == 1) //���ѱ�ȡ��
				{
					 LinkTime =0;
					 machinerec.remealaway = 0;
					 MealoutCurrentPointer=5;
				}
				if( machinerec.remealnoaway == 1) //����ȡ�Ϳڹ���20�뻹δ��ȡ�ߣ���ʱ�ô��ж�
				{
					 LinkTime =0;
					 machinerec.remealnoaway = 0;
					 MealoutCurrentPointer=5;
				}
			}break;			    
			case 5:     /*���û����ݽ��м�һ*/  //?? �����Ҫ���д����˱ң���Ҫ�޸÷���ֵ���ڷ�Χ
			{
				//delay_ms(1000);
				UserAct.Meal_takeout++;//ȡ�����ݼ�
				VariableChage(mealout_already,UserAct.Meal_takeout);	//UI��ʾ
				MealoutCurrentPointer= 0;
				if(UserAct.MealID == 0x01)
				{
					UserAct.MealCnt_1st--;
					if(UserAct.MealCnt_1st==0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
				}
				else if(UserAct.MealID == 0x02)
				{
					UserAct.MealCnt_2nd--;
					if(UserAct.MealCnt_2nd==0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}					
				}
				else if(UserAct.MealID == 0x03)
				{
					UserAct.MealCnt_3rd--;
					if(UserAct.MealCnt_3rd==0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}			
				}				
				else if(UserAct.MealID == 0x04)
				{
					UserAct.MealCnt_4th--; 	
					if(UserAct.MealCnt_4th==0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}		
				}
				//printf("tookone_meal\r\n");
				return tookone_meal;
			}
			default:break;
	  }
	}while(1);
}	

  /*******************************************************************************
 * ��������:CloseCashSystem                                                                     
 * ��    ��:�ر��ֽ����                                                        
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
#define bill_time  600  //500ms�������100msֽ�һ���Ӧ������
bool CloseCashSystem(void)
{
	uint8_t cnt_t=253,money=0,temp=0;
  CloseCoinMachine();			    //�ر�Ͷ�һ�	
	delay_ms(bill_time);
	memset(BillDataBuffer,0xFE,sizeof(BillDataBuffer));
	DisableBills();             //���ò��ر�ֽ�һ�
	do
	{
		cnt_t--;
		delay_ms(1);
		if((RX4Buffer[0]=='0')&&(RX4Buffer[1]=='0'))
		{
			return true;
		}
		else if((RX4Buffer[0]=='F')&&(RX4Buffer[1]=='F'))
		{
			return false;  			
		}
	}while(cnt_t);			 
	 return false; 
}

bool OpenCashSystem(void)
{
	uint8_t cnt_t=254,temp;	
	OpenCoinMachine();    //��Ͷ�һ�	
	delay_ms(bill_time);        //��Ҫ����
	memset(BillDataBuffer,0xFE,sizeof(BillDataBuffer));
	SetBills();           //���ò���ֽ�һ�
	do
	{
		cnt_t--;
		delay_ms(1);
		if((RX4Buffer[0]=='0')&&(RX4Buffer[1]=='0'))
		{
			return true;
		}
		else if((RX4Buffer[0]=='F')&&(RX4Buffer[1]=='F'))
		{
			return false;  			
		}
	}while(cnt_t);	
	return false; 
}

  /*******************************************************************************
 * ��������:ClearingFuntion                                                                     
 * ��    ��:�������� ���������Ƿ��ͳɹ�������û�з��ͳɹ����������͡�ȫ��������Ϻ���㡣                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void ClearingFuntion(void)
{
	 
	RTC_TimeShow();//���ʱ��
	if(TimeDate.Hours == 5 || TimeDate.Hours == 19)
	{
	  // �������� ��
	      ClearingFun();	  //��̨��������
		 // BankFlashCard_Upload(); //��������	 ������ǩ
	}		 		 
}
  /*******************************************************************************
 * ��������:AcountCopy                                                                   
 * ��    ��:�쳣����                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void AcountCopy(void)
{
	UserAct.MealCnt_1st_t= UserAct.MealCnt_1st;
	UserAct.MealCnt_2nd_t= UserAct.MealCnt_2nd;
	UserAct.MealCnt_3rd_t= UserAct.MealCnt_3rd;
	UserAct.MealCnt_4th_t= UserAct.MealCnt_4th;

}

  /*******************************************************************************
 * ��������:PowerupAbnormalHandle                                                                    
 * ��    ��:�����쳣����ֻ��ʾ�����ʶ��������������ʾ�û�������¼                                                                
 *            ������Ҫ�ж��Ƿ�ϵ磬���ж�UserAct.paybacke�Ƿ����0����ʾ�û�ѡ���
              ��Ʒ����δ���Ĳ�Ʒ�����Լ�Ǯ��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 
void PowerupAbnormalHandle(int32_t erro_record)
{
	uint16_t i=0;
//	for(i=0;i<32;i++) //32λ��¼�����쳣��������Ҫ���� ������ͨ������Ա�������
//	{
//		if(erro_record&(1<<i))
//		AbnormalHandle(i);	
//	}
  if(erro_record&(1<<i))  //ֻ����ϵ��ʱ���û��������Ƿ񱣴���
	AbnormalHandle(i);	
}

  /*******************************************************************************
 * ��������:PollAbnormalHandle                                                                   
 * ��    ��:��ѯ                                                                 
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 
void PollAbnormalHandle(void)
{
	uint8_t i=0;
	for(i=32;i>0;i--) //32λ��¼�����쳣��������Ҫ���� ������ͨ������Ա�������
	{
		if(erro_record&(1<<i))
		AbnormalHandle(i);	
	}	
}

  /*******************************************************************************
 * ��������:AbnormalHandle                                                                    
 * ��    ��:�쳣����                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
uint16_t erro_flag=0;
void AbnormalHandle(uint16_t erro)
{	
	erro_flag = erro;
	erro_record |= (1<<erro);
	switch(erro)
	{
		case outage_erro:      //�ϵ� 
			{
				if(UserAct.MoneyBack>0) //�ϵ���Ҫ����UserAct.MoneyBack
				{
					PageChange(Err_interface);
					DisplayAbnormal("E070");
          StatusUploadingFun(0xE070); //״̬����					
				}
				else 
				{
					erro_record &= ~(1<<erro); //�����Ҫ���������쳣��
					//erro_flag= 0;
					return;
				}
			}break;
		case sdcard_erro:     //SD���洢�쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);				
				DisplayAbnormal("E000");
				StatusUploadingFun(0xE000); //״̬����
			}break;
		case billset_erro:    //ֽ�һ��쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);				
				DisplayAbnormal("E010");
				StatusUploadingFun(0xE010); //״̬����
			}break;
		case coinset_erro:      //Ͷ�һ�
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);				
				DisplayAbnormal("E020");
				StatusUploadingFun(0xE020); //״̬����
			}break;
		case coinhooperset_erro:    //�˱һ�
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);				
				DisplayAbnormal("E030");
				StatusUploadingFun(0xE030); //״̬����
				//û�бҿ��˵�ʱ��UserAct.Payback ��Ϊ0
			}break;
		case coinhooperset_empty:   //�����ù�
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E032");
				StatusUploadingFun(0xE032); //״̬����
				//UserAct.MoneyBack
			}break;
		case printer_erro:      //��ӡ���쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E040");
				StatusUploadingFun(0xE040); //״̬����
			}break;
		case cardread_erro:     //�������쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E050");
				StatusUploadingFun(0xE050); //״̬����
			}break;
		case network_erro:     //�����쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E060");
			}break;
		case X_timeout:        //x�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E101");
				StatusUploadingFun(0xE101); //״̬����
				DataUpload(Failed);
			}break;
		case X_leftlimit:      //��������������
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E102");
				StatusUploadingFun(0xE102); //״̬����
				DataUpload(Failed);
			}break;
		case X_rightlimit:     //����Ҷ����������
			{
				DisplayAbnormal("E103");
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE103); //״̬����
				DataUpload(Failed);
			}break;
		case mealtake_timeout: //ȡ�Ϳڴ�������ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E201");
				StatusUploadingFun(0xE201); //״̬����
				DataUpload(Failed);
			}break;
		case Y_timeout:        //y�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E301");
				StatusUploadingFun(0xE302); //״̬����
				DataUpload(Failed);
			}break;
		case link_timeout:     //���ӳ�ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E401");
				StatusUploadingFun(0xE401); //״̬����
				DataUpload(Failed);
			}break;
		case Z_timeout:        //z�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E501");
				StatusUploadingFun(0xE501); //״̬����
				DataUpload(Failed);
			}break;
		case Z_uplimit:        //z������϶�������
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E502");
				StatusUploadingFun(0xE502); //״̬����
				DataUpload(Failed);
			}break;
		case Z_downlimit:      //z����¶�������
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E503");
				StatusUploadingFun(0xE503); //״̬����
				DataUpload(Failed);
			}break;
		case solenoid_timeout: //��ŷ���ʱ  ???��ʱ���쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E601");
				StatusUploadingFun(0xE601); //״̬����
				DataUpload(Failed);
			}break;
		case Eeprom_erro:      //eeprom �쳣
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
				DisplayAbnormal("E711");
				StatusUploadingFun(0xE711); //״̬����
				DataUpload(Failed);
			}break;
		case SendUR6Erro:      //���������쳣��ʱ
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
        DisplayAbnormal("E801");
				StatusUploadingFun(0xE801); //״̬����
				DataUpload(Failed);
      }break;
    case GetMealError:     //��е��5��ȡ������
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
        DisplayAbnormal("E802");
				StatusUploadingFun(0xE802); //״̬����
				DataUpload(Failed);
      }break;
    case MealNoAway:       //���ڳ��Ϳ�20�뻹δ��ȡ��
			{
	      PlayMusic(VOICE_11);	
	      PageChange(Err_interface);
        DisplayAbnormal("E803");
				StatusUploadingFun(0xE803); //״̬����
      }break;
		default:break;
	}
		while(1)
		{
			DealSeriAceptData();
			if(erro_flag==0)
			{
				if(erro>=0x10) //��Ҫ�ж�ʲô����¶��û���������
				{
					ClearUserBuffer();
					SaveUserData();
					//OnlymachieInit();	//ֻ�л�е�ֳ����ʱ��λ��е��
				}
				erro_record &= ~(1<<erro); //һ��ֻ����һ���쳣
				RTC_WriteBackupRegister(RTC_BKP_DR13, erro_record);
				break;
			}
		}
}

  /*******************************************************************************
 * ��������:SaveUserData                                                                    
 * ��    ��:���籣��                                                               
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��23��    
 *******************************************************************************/ 
void SaveUserData(void)
{
	RTC_WriteBackupRegister(RTC_BKP_DR13, erro_record);
	RTC_WriteBackupRegister(RTC_BKP_DR4,  UserAct.MealCnt_1st);
	RTC_WriteBackupRegister(RTC_BKP_DR5,  UserAct.MealCnt_2nd);
	RTC_WriteBackupRegister(RTC_BKP_DR6,  UserAct.MealCnt_3rd);
	RTC_WriteBackupRegister(RTC_BKP_DR7,  UserAct.MealCnt_4th);
	RTC_WriteBackupRegister(RTC_BKP_DR8,  UserAct.Meal_totoal);
	RTC_WriteBackupRegister(RTC_BKP_DR9,  UserAct.Meal_takeout);
	RTC_WriteBackupRegister(RTC_BKP_DR10, UserAct.PayShould);	
	RTC_WriteBackupRegister(RTC_BKP_DR11, UserAct.PayAlready);
	RTC_WriteBackupRegister(RTC_BKP_DR12, UserAct.MoneyBack);
	RTC_WriteBackupRegister(RTC_BKP_DR14,  UserAct.MealCnt_1st_t);
	RTC_WriteBackupRegister(RTC_BKP_DR15,  UserAct.MealCnt_2nd_t);
	RTC_WriteBackupRegister(RTC_BKP_DR16,  UserAct.MealCnt_3rd_t);
	RTC_WriteBackupRegister(RTC_BKP_DR17,  UserAct.MealCnt_4th_t);	
}

  /*******************************************************************************
 * ��������:ReadUserData                                                                    
 * ��    ��:���籣��                                                               
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��23��    
 *******************************************************************************/ 
void ReadUserData(void)
{
	UserAct.MealCnt_1st= RTC_ReadBackupRegister(RTC_BKP_DR4);
	UserAct.MealCnt_2nd= RTC_ReadBackupRegister(RTC_BKP_DR5);
	UserAct.MealCnt_3rd= RTC_ReadBackupRegister(RTC_BKP_DR6);
	UserAct.MealCnt_4th= RTC_ReadBackupRegister(RTC_BKP_DR7);
	UserAct.Meal_totoal= RTC_ReadBackupRegister(RTC_BKP_DR8);
	UserAct.Meal_takeout=RTC_ReadBackupRegister(RTC_BKP_DR9);
	UserAct.PayShould  = RTC_ReadBackupRegister(RTC_BKP_DR10);	
	UserAct.PayAlready = RTC_ReadBackupRegister(RTC_BKP_DR11);
	UserAct.MoneyBack  = RTC_ReadBackupRegister(RTC_BKP_DR12);
	erro_record        = RTC_ReadBackupRegister(RTC_BKP_DR13);
	UserAct.MealCnt_1st_t= RTC_ReadBackupRegister(RTC_BKP_DR14);
	UserAct.MealCnt_2nd_t= RTC_ReadBackupRegister(RTC_BKP_DR15);
	UserAct.MealCnt_3rd_t= RTC_ReadBackupRegister(RTC_BKP_DR16);
	UserAct.MealCnt_4th_t= RTC_ReadBackupRegister(RTC_BKP_DR17);	
}



 /*
  PVD ---- �͵�ѹ���                ��ռ���ȼ�  0      �����ȼ� 0 		���ڱ���sd��
  Ӳ�һ�               �ⲿ�ж�5                 0                1
  ֽ�һ�               ����4                     0                2
  IC��                 ����2                     0                3  
  ��Ļ                 �����ж�3                 0                4
                       CANͨ��                   0                5
  ��ӡ                 ����1                     0                6
  �˱ҵ���ʱ           ��ʱ��4                   1                0
  ��ʱ����ʱ60s        ��ʱ��3                   1                1
  �������             ��ʱ��2                   1                2
  �������             �ⲿ�ж�4                 1                3
  ��ʱ�ɼ��¶Ȳɼ�     ��ʱ��7                   1                4
 */
  /*******************************************************************************
 * ��������:hardfawreInit                                                                     
 * ��    ��:Ӳ����ʼ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 

void hardfawreInit(void)
{
  uint8_t i, j, k;
	Uart6_Configuration();   //��е�� 1 ,2
 //��ʼ�����λ�����ݽṹ��
  for(i = 0; i < 4; i++)
	{
		DefineMeal[i].MealPrice = 0;
		DefineMeal[i].MealCount = 0;
		for(j = 0; j < 15; j++)
		{
			for(k = 0; k < 3; k++)
			{
				DefineMeal[i].Position[j][k] = 0; 
			}
		}	
	}
 	 UserAct.PayForCoins     = 0;
	 UserAct.PayForBills     = 0;
	 UserAct.PayForCards     = 0;
	 UserAct.PayAlready      = 0;
   SystemInit();
//	 delay_ms(30000); //�ϵ�ȴ�·��������
	 PVD_Configuration();        //�������ʼ�� 0 , 0
	 Uart4_Configuration();     //ֽ�һ����ڳ�ʼ�� 1, 2
	 Uart1_Configuration();	    //��ӡ�����ڳ�ʼ��
	 Uart3_Configuration();	    // ��������ʼ��  0 , 3
	 Uart2_Configuration();	    //����ͨ������������ 1 , 0
	 Uart5_Configuration();		//���紮�ڳ�ʼ�� 1 , 1
	 TIM2_Init();		        //���
	 TIM3_Init();		        //��Ʒ����ѡ�񵹼�ʱ
	 TIM4_Init();		        //��ӡ����ʱ5S
	 TIM5_Init();		        //��е�ֵ���ʱ
	 TIM7_Init();				    //���ﳵ���浹��ʱ
   InitCoins();		        //Ͷ�һ���ʼ��
   InitMiniGPIO() ;		   //�˱���ʼ��	 
	 InitVoice();             //������ʼ��
	 MyRTC_Init();              //RTC��ʼ��
	 //IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //�򿪿��Ź�
	 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //�򿪿��Ź�
	 IWDG_SetPrescaler(IWDG_Prescaler_128); //40K /128 =312 = 0X0138
	 IWDG_SetReload(0x0138); // 1S
	 IWDG_Enable();
	 OpenTIM2();
	 PageChange(HardwareInit_interface); //Ӳ����ʼ������
  //PageChange(Logo_interface); //�ظ�һ�ξͿ��Գɹ�

	 SPI_FLASH_Init();          //Flash��ʼ��
	 //SPI_FLASH_Init();          //�ظ���ʼ������
   SPI_FLASH_BufferRead(FloorMealMessageWriteToFlash.FlashBuffer, SPI_FLASH_Sector0 , FloorMealNum*6);//��ȡ����Ĳ�Ʒ
	 //WriteCoins();
   ReadCoins();//��ȡ�ж���Ӳ��	 
	 //VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ����
	 for(i=0;i<90;i++)
	 {
	   if(FloorMealMessageWriteToFlash.FlashBuffer[i] == 0xff)
	   FloorMealMessageWriteToFlash.FlashBuffer[i]    = 0 ;
	 }
	 WriteMeal();  //д���Ʒ����
	 StatisticsTotal(); //����ĳ�����Ҫʹ��  	
	 ReadUserData();  //��Ҫ�������ݴ����ж�
	 if(erro_record!=0) //���д����¼����Ҫ���д���
	 {
		 PowerupAbnormalHandle(erro_record);
	 }
	 else
	 {
		 ClearUserBuffer(); //���֮ǰ��ȡ������
	 }
}														 
