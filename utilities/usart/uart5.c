#include "stm32f4xx.h"

void Uart5_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//����ʱ��ʹ�� 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//����ʱ��ʹ�� 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);//���Ӹ�������  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ������1��GPIO  
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ������1��GPIO  
		 
  	USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����ģʽ
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ���������
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//˫��ģʽ
	  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
  	USART_Init(UART5, &USART_InitStructure);	
    USART_ClearFlag(UART5, USART_FLAG_TC);//�崫����ɱ�־
    USART_Cmd(UART5, ENABLE);
  	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ƕ�����ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//Ƕ��ͨ��ΪUSART6_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ�� 
	NVIC_Init(&NVIC_InitStructure);
}





 void Uart5Send(unsigned char *p,unsigned char lenght)
{
     uint8_t i=0;
	 for(i=0;i<lenght;i++)
	 {
	  
	    UART5->DR = (u8) p[i];       
    	while((UART5->SR&0X40)==0);//ѭ������,ֱ���������   
    	    
	}
}	

     /*******************************************************************************
 * ��������:UART5_IRQHandler                                                                     
 * ��    ��:�жϳ��� �����������֮������ݽ���                                                                
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
unsigned char   F_RX1_Right ;
uint8_t  rx1Buf[512];
uint16_t  rx1BufIndex;
uint8_t  F_RX1_SYNC;	  //ͬ�� tou ��������ͷͷ
uint8_t  F_RX1_VAILD;    //�յ���Ч����
long     rx1DataLen;  
long     CrcValue;
void UART5_IRQHandler(void)
{
	unsigned char res = 0;
	unsigned char j =0 ;
	#define DEF_RX1_Head   0x02
	#define DEF_RX1_End    0x03	
	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//���ݽ���ɨ��
	{	
	  USART_ClearITPendingBit(UART5,USART_FLAG_RXNE);//����ձ�־
    res=UART5->DR;
	  if(!F_RX1_SYNC)
	  {		
		  if(res ==DEF_RX1_Head )//�ж��Ƿ��Ǵ���֡ͷ
		  {
			  F_RX1_SYNC = 1;	 //����ͬ����־
			  rx1BufIndex = 0; //��������������
			  F_RX1_VAILD = 1; //�յ���Ч����
			  CrcValue = 0;
			  F_RX1_Right = 0 ;
		  }	
    }
 	  else 
    {	
	    rx1Buf[rx1BufIndex]=res;
	    if(rx1BufIndex==0x04)
	    {
	      rx1DataLen   = rx1Buf[2] << 8 | rx1Buf[3] ; 	 /*�������ݳ���*/	
	    }
	    if(F_RX1_VAILD == 0x01)/*���ܵ�������*/
	    {
		    if((rx1BufIndex == rx1DataLen + 3)&&(rx1DataLen != 0x00)) /*�жϽ��ܵ����ݳ����Ƿ��Ӧ�������ܵ����ݲ�Ϊ0*/
		    {
			    F_RX1_VAILD = 0;  /*???��־һ�����ܵ���������ȷ��*/
			  }
		    /*У������*/
		    if(1)
			  {
	   		  CrcValue ^= res;
				  for(j=0;j<8;j++)
				  {
				    if(CrcValue&0x0001)
					  { 
						  CrcValue >>=1 ;
						  CrcValue ^=0x08408 ;
					  }
					  else 
						  CrcValue >>=1 ;
				  }
		    }	 
		  }
		  if(rx1BufIndex == rx1DataLen + 6) /*?���������*/
		  {	 
		  	/*?�жϽ������ж�CRC�Ƿ���ȷ*/	
		    if(rx1Buf[rx1DataLen +4] == DEF_RX1_End && (rx1Buf[rx1DataLen +5]<<8| rx1Buf[rx1DataLen +6]) == CrcValue)
			  {
			    F_RX1_SYNC = 0;
				  rx1BufIndex = 0;
				  F_RX1_VAILD = 1;
				  CrcValue = 0;
				  F_RX1_Right = 1 ;
				  return ;
			  }
		  }
	    rx1BufIndex++;
    }
	}  
}
