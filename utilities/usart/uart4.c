#include "stm32f4xx.h" 
#include "string.h"
#include "bill.h"
void Uart4_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//����ʱ��ʹ�� 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);//���Ӹ�������  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ������1��GPIO   
  	USART_InitStructure.USART_BaudRate = 9600;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����ģʽ
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ���������
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//˫��ģʽ
	  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
  	USART_Init(UART4, &USART_InitStructure);
  	USART_Cmd(UART4, ENABLE);
  	USART_ClearFlag(UART4, USART_FLAG_TC);//�崫����ɱ�־
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ƕ�����ȼ�����Ϊ1
	  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//Ƕ��ͨ��ΪUSART6_IRQn
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ�� 
	  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * ��������:UART4_IRQHandler                                                                     
 * ��    ��:�жϳ��� ���ڵõ�ֽ�һ�������                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
char         RX4Buffer[40]={0};	  /*����4�������ݻ����� */
uint8_t 	   rx4DataLen =0 ;  /*����4�������ݸ���*/
uint8_t      rx4ack;		      /*��������Ӧ��*/
uint8_t      rx4Check ;		    /*��������У��λ*/
unsigned char  BillDataBuffer[20]={0}; //��Ϊת����ɵ�����
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//���ݽ���ɨ��
	{  
		USART_ClearITPendingBit(UART4,USART_FLAG_RXNE);//����ձ�־
    RX4Buffer[rx4DataLen]=UART4->DR;
		if(RX4Buffer[rx4DataLen++]==0x0A) //�����յ�LFʱ�����������ֹ
		{
		   if(StringToHexGroup(BillDataBuffer, RX4Buffer, rx4DataLen)) //ת������
		   {
		     rx4Check  = 1 ;  // ��Ҫ�����
         rx4DataLen= 0 ;  //
		   }
		   else
		   {
          memset(RX4Buffer,0,rx4DataLen);				 
		      rx4DataLen= 0 ; 
		   }
		}
	}  
}

