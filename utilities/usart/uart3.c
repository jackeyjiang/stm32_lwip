#include "bsp.h"
 
void Uart3_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//����ʱ��ʹ�� 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);//���Ӹ�������  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ������1��GPIO  
	 
  	USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����ģʽ
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ���������
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//˫��ģʽ
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  	USART_Init(USART3, &USART_InitStructure);	
    USART_ClearFlag(USART3, USART_FLAG_TC);//�崫����ɱ�־
  	USART_Cmd(USART3, ENABLE);
  	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ƕ�����ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//Ƕ��ͨ��ΪUSART3_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ�� 
	NVIC_Init(&NVIC_InitStructure);
}

void Uart3_Send(const uint8_t *p,uint8_t length)
{
   uint8_t i=0;
	 USART_ClearFlag(USART3,USART_FLAG_TC);
	 for(i=0;i<length;i++)
	 {	
		  //while((USART3->SR&0X40)==0);//ѭ������,ֱ���������    
	    USART_SendData(USART3, (u8) p[i]);   
    	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    	    
	 }
}
void Uart3_Sent(const char *p,uint8_t length)
{
   uint8_t i=0;
	 USART_ClearFlag(USART3,USART_FLAG_TC);
	 for(i=0;i<length;i++)
	 {	  
	    USART3->DR = (u8) p[i];       
    	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    	    
	 }
}
  /*******************************************************************************
 * ��������:_LCD_Disable_RxInit                                                                     
 * ��    ��:�ر�LCD�����ж�                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void _LCD_Disable_RxInit(void)
{
  	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
}
  /*******************************************************************************
 * ��������:_LCD_Enable_RxInit                                                                     
 * ��    ��:��LCD�����ж�                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void _LCD_Enable_RxInit(void)
{
  	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
}



#define RX3BUF_SIZE 200				 /*����FIFO���������*/  
unsigned char RX3_BUFF[RX3BUF_SIZE];	 /*����FIFO����������*/
unsigned char UART_InpLen =0;        /*����FIFO�ڴ���ȡ���ֽ���*/
unsigned int  RX_IndexR =0;          /*����FIFO�Ķ�ָ��*/
unsigned int  RX_IndexW =0;          /*����FIFO�Ķ�ָ��*/
 /*******************************************************************************
 * ��������: USART3_GetChar()
 * ��    ��: �Ӵ��ڶ�ȡһ���ֽ����ݣ��ӻ�����ж�ȡ1�ֽ��ѽ��ܵ����ݣ�
 *
 * ��    ��: *chr����ȡ��������ŵĵ�ַָ��
 * ��    ��: ����1��ʾ��ȡ�ɹ�������0��ʾ��ȡʧ��
 * ��    ��: char
 * �޸�����: 2014��3��13��   
*******************************************************************************/
char USART3_GetChar(unsigned char *Chr)
{
	if(UART_InpLen==0)return(0);    //���FIFO�������ݣ�����0
	//_DINT();											//�漰FIFO����ʱ�������жϣ�����ָ�����
	UART_InpLen--;                  //����ȡ�����ֽ�����1
	*Chr =RX3_BUFF[RX_IndexR];			//����ȡָ���ȡһ���ֽ���Ϊ����ֵ
	if(++RX_IndexR>=RX3BUF_SIZE)    //��ȡָ����������ж��Ƿ��±�Խ��
	{
		RX_IndexR =0;                 //���Խ����дָ���0(ѭ������)
	}
	//_EINT();                      //FIFO������ϣ��ָ��ж�����
	return(1); 											//���سɹ���־	
}
 /*******************************************************************************
 * ��������: USART3_GetCharInRxBuf()
 * ��    ��: ��ȡFIFO���ѽ��ܵ������ֽ���
 *
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: FIFO�����ݵ��ֽ���
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
unsigned int UART3_GetCharsInRxBuf(void)
{
	return(UART_InpLen);           //����FIFO�����ݵ��ֽ���
}

 /*******************************************************************************
 * ��������: UART0_ClrRxBuf()
 * ��    ��: �������FIFO��
 *
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: ��
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
void UART3_ClrRxBuf(void)
{
	//_DINT();
	memset(RX3_BUFF,0,sizeof(RX3_BUFF));
	//_EINT();

}



 /*******************************************************************************
 * ��������: USART3_IRQHandler
 * ��    ��: �жϴ���������ѹ�뻺��
 *           �򵥵�����֡����
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: ��
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//���ݽ���ɨ��
	{
  		
	UART_InpLen++;
	RX3_BUFF[RX_IndexW]=USART_ReceiveData(USART3);
	if(++RX_IndexW >=RX3BUF_SIZE)
	{
		RX_IndexW =0;
	}
	}
}
