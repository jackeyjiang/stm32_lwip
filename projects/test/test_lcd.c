 /**
  ******************************************************************************
  * �ļ�: test_lcd.c
  * ����: jackey
  * �汾: V1.0.0
  * ����: DWIN ������������ʵ�ִ�����Ļ�л������ݶ�ȡ
  *       test_lcd
  ******************************************************************************
  *
  *                  	��ΪSTM32F407������ģ��
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
uint8_t Current= 0x01;        //״̬������
uint8_t LinkMachineFlag =0;	  //���е�����ӱ�־��0��ʾû���ӣ�1��ʾ����
uint8_t waitmeal_status=0;    //�ȴ�ȡ��״̬
int32_t erro_record=0x00000000; //������λ

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x0001, 0x0002, 0x0003,};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

int main(void)
{
	uint16_t temp = 0;
	hardfawreInit(); //Ӳ����ʼ��
	PageChange(OnlymachieInit_interface);
  OnlymachieInit();  //��е�ֳ�ʼ��
   /*������  ���ʱ�䣬���±���ʱ��*/
	PageChange(SignInFunction_interface);
  if(!EchoFuntion(RTC_TimeRegulate)) AbnormalHandle(network_erro);
	/*����ǩ��*/
	if(!SignInFunction())       AbnormalHandle(signin_erro);
  SendtoServce();  //�ϴ�ǰ���������
	/*����ͨǩ��*/
	PageChange(Szt_GpbocAutoCheckIn_interface);
	if(!Szt_GpbocAutoCheckIn()) AbnormalHandle(cardchck_erro);
	if((CoinsTotoalMessageWriteToFlash.CoinTotoal<50)||( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)== 0)) 	
	AbnormalHandle(coinhooperset_erro); //������Ӳ����С��50 �� Ӳ�һ��������� ���� 
	PageChange(Logo_interface);	
	delay_ms(1500);
	if(!CloseCashSystem())  AbnormalHandle(billset_erro);	
	DispLeftMeal();             //��ʾ��Ʒ����	
	PageChange(Menu_interface); //��ʾѡ�ͽ���
	
	while(1)
  {
		DealSeriAceptData();
		manageusart6data();   //
    switch(Current)
	  {
	    case current_temperature: /*�¶ȴ�����*/
			{
				StateSend();
				if(LinkTime >=5)
				{
					temp =0;
					temp = OrderSendLink();  //Ϊ1�ɹ���Ϊ0ʧ��
					VariableChage(current_temprature,Temperature); //5Sһ��
				}
				//��ʾ����ʱ,���Ը��߱��Ϊ��ѡ�͵���ʱ���и���
				if(UserAct.MealID)
				{
					if(WaitTime==0)
					{					 
						PageChange(Menu_interface);//��ʱ�˳��û���Ʒ����ѡ�����
						WaitTimeInit(&WaitTime);
					}
					else if(WaitTime!=60)
					{
						VariableChage(count_dowm,WaitTime); //��С�ĳ���������ն���ֱ�ӽ���
					}
				}
			}break;
	    case waitfor_money:	 /*�ȴ���Ǯ*/
			{
				SaveUserData();
        if( WaitPayMoney()==Status_OK)
				{
          PageChange(TicketPrint_interface);/*��ӡ������ʾ������*/
					PlayMusic(VOICE_7);					
					CloseTIM3();
					CloseTIM7();					
					//�ı��û���ѡ�͵�����
					MoneyBack =UserAct.MoneyBack *100 ;  /*����10��*/
			    mealvariety =0; 
					UserAct.Cancle= 0x00; //�������
					Current= hpper_out;
					UserAct.Meal_takeout= 0;	
          VariableChage(mealout_already,UserAct.Meal_takeout);	//UI��ʾ					
					if(UserAct.PayType == '1')
					{
						delay_ms(3000);
						if(!CloseCashSystem()){};// printf("cash system is erro1\r\n");  //�ر��ֽ����
					}
			  }
			}break;
      case hpper_out:	 /*�˱�״̬*/
			{
		    if(UserAct.MoneyBack >0) //��Ҫ�ұҵ�ʱ�����
		    {
					UserAct.MoneyBack = SendOutN_Coin(UserAct.MoneyBack);					
					if(ErrorType ==1)  //�˱һ��ޱҴ��󣬻�δ��������״̬������
					{
						erro_record |= (1<<coinhooperset_empty);
					}
				}
				else  //�����ұҵ�ʱ��ֱ�ӽ������״̬,
				{
					if(UserAct.Cancle== 0x00) //�ж��ǲ���ȡ������
					{
					  Current= meal_out; 
						WaitTime=5;//5S��ʱ   
	       	  OpenTIM4(); 
						break;
					}
					else if(erro_record>0x10) //�˱Һ���쳣����
					{
						Current= erro_hanle;
					}
					else
					{
						UserAct.Cancle= 0x00;
						Current= current_temperature;
					}
				}
			}break;
	    case meal_out:	 /*����״̬�����ڳ��ͣ��ѳ�һ�ֲ�Ʒ���������*/
			{
				SaveUserData();
				if(WaitTime==0)
				{
		      PlayMusic(VOICE_8);			
				}
				waitmeal_status= WaitMeal();       
			  if(waitmeal_status == takeafter_meal) //�������
				{
					PageChange(Menu_interface);
					//������ϣ��������¶ȴ���
          UserAct.MoneyBack   = 0;
          UserAct.PayAlready  = 0;
          UserAct.PayForBills = 0;
          UserAct.PayForCoins = 0;
          UserAct.PayForCards = 0;
          UserAct.Meal_takeout= 0;
          UserAct.Meal_totoal = 0;
					//������ﳵ
					ClearUserBuffer();          
					Current = current_temperature;
				}
				else if(waitmeal_status == tookkind_meal) //ȡ��һ�ֲ�Ʒ
				{
					PageChange(Mealout_interface);//��֪����Ҫ��Щʲô
					Current = data_upload;	
				}
				else if(waitmeal_status == tookone_meal)  //ȡ��һ����Ʒ
				{
					PageChange(Mealout_interface);
					Current = data_upload;					
				}
        else if(waitmeal_status == takemeal_erro)
				{
					//��е�ָ�λ����ȴ�����		
					NewCoinsCnt= 0; 
          UserAct.MoneyBack =UserAct.MealCnt_1st *price_1st+UserAct.MealCnt_2nd *price_2nd+UserAct.MealCnt_3rd *price_3rd+UserAct.MealCnt_4th*price_4th; //���㵥�ܼ�		
					OldCoinsCnt= UserAct.MoneyBack;
					PageChange(Err_interface);
					UserAct.Cancle= 0x01;
					Current = hpper_out;	
          break;					
				}			
			}break;
	    case data_upload:	 /*�����ϴ�*/
	    {  		
        DataUpload(Success);//����UserAct.ID �ж���Ҫ�ϴ�������
			  Current = meal_out;		
	    }break ;
      case erro_hanle: /*�쳣״̬����*/
      {
				PollAbnormalHandle(); //�쳣���� һֱ�����쳣�������
				StatusUploadingFun(0xE800); //����󷵻�����	
        Current = current_temperature;					
		  }
	  }
  }
}
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

}
#endif

