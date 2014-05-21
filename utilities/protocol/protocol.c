#include "stm32f4xx.h"
#include "bsp.h"
#include "uart6.h"


/*��Ϊ���е�ֶԽӵ�Э�� ��ʹ��USART6ͨѸ ,
�����͵������NACK��0x15)ʱ�����ֻ����ԭ�����е�֣�
��е��Ҳͬ��
0x0d ��ʾ�س�
0x0a ��ʾ����
*/
uint8_t RetryFre;

uint8_t LinkTime;

ErrorFlagInf ErFlag;
Urart6RecFlagInf machinerec;
  /***********************************************
 ���ڶ�ʱʱ�䡣
 *************************************************/
//ͨ�ö�ʱ��6��ʼ��,��84MHz��TIM6ʱ��ԴΪ168MHz/2������TIM_scale��Ƶ����Ϊ����ʱ��
/*******************************************************************************
* Function Name  : TIM5_Init
* Description    :��ʱ��5��ʼ��
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
 void TIM5_Init(void)//TIM_PeriodΪ16λ����	   //����1�����ж�һ��
{
  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//����ʱ��ʹ��  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;					
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;//��ռ���ȼ����
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = 19999;//��������װֵ
  	TIM_TimeBaseStructure.TIM_Prescaler = 4199;
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ʽ
  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM5, (4200-1), TIM_PSCReloadMode_Immediate);//����16λʱ�ӷ�Ƶϵ��,��������ģʽ
	  TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* �������жϱ�־ */
  	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//�����ж�
  	TIM_Cmd(TIM5, DISABLE);

}

    /*******************************************************************************
* Function Name  : OpenTIM5
* Description    : �򿪶�ʱ��5
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void OpenTIM5(void)
{
   TIM_Cmd(TIM5, ENABLE);
}

    /*******************************************************************************
* Function Name  : CloseTIM5
* Description    : �رն�ʱ��5
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void CloseTIM5(void)
{
   TIM_Cmd(TIM5, DISABLE);
}

/*����ACK*/
void SendAck(void)
{
  USART_SendData(USART6,0x06 );//����1����һ���ַ�
  while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
}



/*����NACK*/

void SendNack(void)
{
  USART_SendData(USART6,0x15 );//����1����һ���ַ�
  while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
}




/*�·���е�ֳ�ʼ���������ʱ�ã���е���յ��󷵻�0x06,�쳣ʱ����E***�ȴ������*/
void MachineInit(void)
{
  uint8_t Cmd[6]={'I','0','0','0',0x0d,0x0a};
  uint8_t i;
  
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}


/*����ȡ������,0�б�ʾA�У�1�б�ʾB�У�2�б�ʾC�У�����2λ10���Ʊ�ʾ��������е�ֵĲ��иߵ�λ֮��
��е���յ��󷵻�ACK(0x06)*/
void SendCoord(uint8_t floor,uint8_t row)
{
  uint8_t Cmd[6]={'M','0','0','A',0x0d,0x0a};
  uint8_t i;

  
  Cmd[1] = Cmd[1]+(floor + 1)/10;
  Cmd[2] = Cmd[2]+ (floor +1)%10;
  Cmd[3] = Cmd[3] + row;
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}



/*��ȷ�ϻ�е������ָ���������0x06,�쳣ʱ����E000*/
void SendLink(void)
{
  uint8_t Cmd[6]={'P','0','0','0',0x0d,0x0a};
  uint8_t i;
  
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}



/* ����ȡ���źţ�����ȡ���󣬴��ź���Ч����������0x06,
����������*/
void StopSell(void)
{
  uint8_t Cmd[6]={'C','0','0','0',0x0d,0x0a};
  uint8_t i;
  
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}



/*ȡ���źţ���е���յ����źź󣬷���0x06,���Ӳͼ���ȡ�߲ͣ����Ѳ��͵����Ϳ�*/
void GetMeal(void)
{
  uint8_t Cmd[6]={'G','0','0','0',0x0d,0x0a};
  uint8_t i;
  
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}


/*�����¶�,*/
void SetTemper(uint8_t temper)
{
  uint8_t Cmd[6]={'H','D','0','0',0x0d,0x0a};
  uint8_t i;

  Cmd[2] = Cmd[2] + (temper)/10;
  Cmd[3] = Cmd[3] + (temper)%10;
  
  for(i=0;i<6;i++)
  {
    USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}



/*��������Usart6����*/
void manageusart6data(void)
{
  if(Usart6DataFlag ==1)
	{
		Usart6DataFlag = 0;
		switch (Usart6Buff[0])
		{
			case 'E':
				if( Usart6Buff[1] == 1)
				  {
				    if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E101 = true;
								AbnormalHandle(X_timeout);
							}
							else if( Usart6Buff[3] == 2 )
							{
								ErFlag.E102 = true;
								AbnormalHandle(X_leftlimit);
							}
							else if( Usart6Buff[3] == 3 )
							{
								ErFlag.E103 = true;
								AbnormalHandle(X_rightlimit);
							}
							else
							{
							}
						}
						else
						{
						}
				  }
				else if( Usart6Buff[1] ==2)
				{
					if( Usart6Buff[2] == 0 )
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E201 = true;
							AbnormalHandle(mealtake_timeout);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else if( Usart6Buff[1] ==3)
				{
					if( Usart6Buff[2] == 0 )
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E301 = true;
							AbnormalHandle(Y_timeout);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else if( Usart6Buff[1] ==4)
				{
					if( Usart6Buff[2] == 0 )
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E401 = true;
							AbnormalHandle(link_timeout);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else if( Usart6Buff[1] ==5)
				{
					if( Usart6Buff[2] == 0 )
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E501 = true;
							AbnormalHandle(Z_timeout);
						}
						else if( Usart6Buff[3] == 2 )
						{
							ErFlag.E502 = true;
							AbnormalHandle(Z_uplimit);
						}
						else if( Usart6Buff[3] == 3 )
						{
							ErFlag.E503 = true;
							AbnormalHandle(Z_downlimit);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else if( Usart6Buff[1] ==6)
				{
					if( Usart6Buff[2] == 0 )
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E601 = true;
							AbnormalHandle(solenoid_timeout);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else if( Usart6Buff[1] ==7)
				{
					if( Usart6Buff[2] == 0 )
					{
						
					}
					else if(Usart6Buff[2] == 1 ) 
					{
						if( Usart6Buff[3] == 1 )
						{
							ErFlag.E711 = true;
							AbnormalHandle(Eeprom_erro);
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else
				{
				}
				 break;
			case 'D':
				if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0'))
			  		{
			  		  //����
			  		  machinerec.redoor = 0;
	
			  		}
				else  if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '1'))
				{
					//����
					machinerec.redoor = 1;
				}
				else
				{
				}
				break;
			case 'N':
				if((Usart6Buff[1]== 'O')&&(Usart6Buff[2]== 'R')&&(Usart6Buff[3]== '0'))
				  		{
				  		  //�������ԭ�㣬������λ��
						  machinerec.rerelative = 1;
				  		}
				else
					{
					}
				break;
			case 'S':
				if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0'))
			  		{
			  		  //����ȡ�͵�
					  machinerec.regoal = 1;
			  		}
				else
				{
				
				}
				break;
			case 'O':
				if((Usart6Buff[1]== 'U')&&(Usart6Buff[2]== 'T')&&(Usart6Buff[3]== '0'))
			  		{
			  		  //���ѵ�����Ϳ�
					  machinerec.retodoor = 1;
			  		}
				else
				{
				
				}
				break;
			case 'F':
				if((Usart6Buff[1]== 'I')&&(Usart6Buff[2]== 'N')&&(Usart6Buff[3]== '0'))
			  		{
			  		  //���ѱ�ȡ��
					  machinerec.remealaway = 1;
			  		}
				else
				{
				
				}
				break;
			case 'T':
				if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
					{
						//����ȡ�Ϳڹ���20�뻹δ��ȡ��
					machinerec.remealnoaway = 1;
					}
				else if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '1')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
					{
						//ȡ��5���˻�δȡ����
					machinerec.reenablegetmeal = 1;
					}
				else if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '2')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
					{
						//ȡ��5���˻�δȡ����
					machinerec.reenablegetmeal = 1;
					}
				else if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '3')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
					{
						//ȡ��5���˻�δȡ����
					machinerec.reenablegetmeal = 1;
					}
				else if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '4')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
					{
						//ȡ��5���˻�δȡ����
					machinerec.reenablegetmeal = 1;
					}
					else
				{
				
				}
				break;
			case 'A':
				if( Usart6Buff[1] == '+' )
				  {
				    TemperSign =0;
				  }
				else if( Usart6Buff[1] == '-' )
				{
					TemperSign =1;
				}
				else
				{
				}
				Temperature =  (Usart6Buff[2] - 48)*10 + (Usart6Buff[3] - 48);
				break;
			default : 
		  	break;
		}
		Usart6Index =0;
		memset(Usart6Buff,0,6);
	}
}






/*����1��ʾ���ӳɹ�������0��ʾ����ʧ��*/
uint8_t OrderSendLink(void)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	SendLink();
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			SendLink();			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0; 
			return 0;
		}
		
	}
	
}



/*����1��ʾ��ʼ���ɹ�������0��ʾ��ʼ��ʧ��*/
uint8_t OrderMachineInit(void)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	MachineInit();
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			MachineInit();			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 0;
		}
		
	}
	
}

/*����1��ʾ��������ɹ�������0��ʾ��������ʧ��*/
uint8_t OrderSendCoord(uint8_t floor,uint8_t row)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	SendCoord(floor,row);
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ ??? 
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			SendCoord(floor,row);			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 0;
		}
		
	}
	
}


/*����1��ʾֹͣ���۳ɹ�������0��ʾֹͣ����ʧ��*/
uint8_t OrderStopSell(void)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	StopSell();
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			StopSell();			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 0;
		}
		
	}
	
}


/*����1��ʾȡ������ɹ�������0��ʾȡ������ʧ��*/
uint8_t OrderGetMeal(void)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	GetMeal();
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			GetMeal();			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 0;
		}
		
	}
	
}


/*����1��ʾ���������¶ȳɹ�������0��ʾ��ʾ���������¶�ʧ��*/
uint8_t OrderSetTemper(uint8_t inputtemper)
{
	uint8_t RetryFre;
	
	LinkTime =0;
	RetryFre =0;
	SetTemper(inputtemper);
	delay_ms(5);
	while(1)
	{
		if( machinerec.reack ==1)	  //ack
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 1;
		}
		
		if( LinkTime >1)  //��ʱ
		{
			LinkTime =0;
			return 0;
		}
		
		if(machinerec.renack ==1)  //nack
		{
			machinerec.reack = 0;
			machinerec.renack = 0;
			RetryFre ++;
			SetTemper(inputtemper);			
		} 
		
		if( RetryFre>=3)
		{
			LinkTime =0;
			machinerec.reack = 0;
			machinerec.renack = 0;
			return 0;
		}
		
	}
	
}



/*��е�ֳ�ʼ��*/
void OnlymachieInit(void)
{
  uint8_t temp;
  uint8_t InitOkflag =0;
  uint8_t mancineinitflag;
  
  LinkTime =0;
	OpenTIM5();

	Usart6Index =0;  //��е�ֽ�������
	memset(Usart6Buff,0,6);
  
  machinerec.rerelative = 0;
  
  while(1)
  {
    temp =0;
    temp = OrderSendLink();  //Ϊ1�ɹ���Ϊ0ʧ��
    if( temp == 1)
    {
      mancineinitflag = OrderMachineInit();
      
      if(mancineinitflag ==1)  //����ͳɹ�
      {
        LinkTime =0;
        while(1)
        {
          if(Usart6DataFlag ==1)  //���е����ͨѸ
          {
            manageusart6data();
          }
          
          if(machinerec.rerelative == 1)   //�������λ��
          {
            InitOkflag =1;
            break;
          }
          
          if(LinkTime >= 60)
          {
            //printf("��е�ֳ�ʼ����ʱ\r\n");
            break;
          }
        }
      }
    }
    
    if(LinkTime >= 60)
    {
      //printf("��е�����ӳ�ʱ\r\n");
      break;
    }
    
    if(InitOkflag ==1 )
    {
      //printf(" ��е�ֳ�ʼ���ɹ�\r\n");
      break;
    }
   
  }
  
}
