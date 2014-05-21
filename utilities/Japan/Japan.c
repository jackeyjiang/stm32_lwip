#include "stm32f4xx.h"
#include "bsp.h"
#include "japan.h"
uint8_t      RX4Buffer[40];	  /*����4�������ݻ����� */
uint8_t 	   rx4DataLen =0 ;    /*����4�������ݸ���*/
uint8_t      rx4ack;		  /*��������Ӧ��*/
uint8_t      rx4Check ;		  /*��������У��λ*/


unsigned char  Rev_Money_Flag ; 
 /*******************************************************************************
 * ��������:mem_set_0                                                                      
 * ��    ��:��������������                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
static void mem_set_0(unsigned char *dest, const long s_len)
{
    unsigned char j0;

    for(j0 = 0; j0 < s_len; j0++)  dest[j0] = 0;
}
 /*******************************************************************************
 * ��������:Uart4_Card                                                                      
 * ��    ��:����ͨ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
void Uart4SendData(uint8_t *p,uint8_t sizeData)
{
      uint8_t i;		   
	  for(i=0; i<sizeData; i++)
	  {
	    printf("p[%d]=%x\r\n",i, p[i]);
      USART_SendData(UART4, p[i]);//����1����һ���ַ�
		  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
 	  }
}
 /*******************************************************************************
 * ��������:SetJapanBills                                                                     
 * ��    ��:���ý���10  20 5  ��Ǯ                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void SetJapanBills(void)
{
    unsigned char bufCmd[6] = {0x34, 0x00, 0x1f, 0x00, 0x00};
	uint8_t i;
	for(i = 0; i < 5; i++)
	{
	    USART_SendData(UART4, bufCmd[i]);//����1����һ���ַ�
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
	}
}
 /*******************************************************************************
 * ��������:ack                                                                     
 * ��    ��:У��ӻ��Ƿ�����ACK                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
unsigned char  ACKFun(void)
{
   unsigned char   WaitOut = 0xff ;
   do
   {	
      WaitOut --;
	 if(WaitOut == 0x00) 
	  break;
	  delay_us(1);
   } while(!rx4ack);
   if(WaitOut == 0x00)
   return 1;
   rx4ack = 0 ;
   return 0 ;
}

unsigned char  Check(void)
{
   unsigned char   WaitOut = 0xff ;
   do
   {   WaitOut --;
	 if(WaitOut == 0x00) 
	  break;
	  delay_ms(1);
   } while(!rx4Check);
   if(WaitOut == 0x00)
   return 1;
   rx4Check = 0 ;
   return 0 ;
}
 /*******************************************************************************
 * ��������:Polls                                                                     
 * ��    ��:���������ôӻ���Ǯ                                                                
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void  PollFun(void)
{
   
	USART_SendData(UART4, 0x01);//����1����һ���ַ�
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ�������� 
}
void   BackBill(void)
{
 
}
  /*******************************************************************************
 * ��������:ReadBills                                                                     
 * ��    ��:���ý���10  20 5  ��Ǯ                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 00 20 A0 03 90 09 5C    //����1Ԫ
 00 20 A0 03 91 09 5D 	 //����5Ԫ
 00 20 A0 03 92 09 5E    //����10Ԫ
 00 20 A0 03 93 09 5F    //����20Ԫ  

 00 20 A0 02 09 CB        //ϵͳæ������ ����Ͷ��
 00 20 A0 02 0A CC        //û��Ǯ��Ǯ�䡣
 00 20 A0 02 0B CD        //Ǯû�б�Ͷ���ȥ                                                           
 *******************************************************************************/
#define Normal 1 //ֽ�һ���ʼ���ɹ�	30 06 09
#define Abnormal 2  //ֽ�һ����ڹر�״̬ 30 09
#define Erro 3     //ֽ�һ����ص���0xff
#define BillInEscrow 4 //Ǯ���ݴ�����

#define		BillStacked  1
#define		EscrowRequest  2
#define		BillReturned  3
#define		DisabledBillRejected  4	

char BillStatus=0; //ֽ�һ�״̬
char BillRunStatus;	  //ֽ�һ�����״̬
extern unsigned char BillDataBuffer[20];
uint8_t  ReadBill(void)
{ 
  uint8_t	BillValue=0;
  if(BillDataBuffer[0]==0x30) //money form cash macthin 
	{
	   if(BillDataBuffer[1]&0x80)	//cash was valuable
		{
			switch(BillDataBuffer[1]&0x70)    //cash matchine status
			{
				case 0x00: BillStatus =BillStacked;break;
				case 0x10: BillStatus =EscrowRequest;break;
				case 0x20: BillStatus =BillReturned;break;
				case 0x40: BillStatus =DisabledBillRejected;break;
				default:break ;
				//return BillValue; //
			}
			if(BillStatus ==BillStacked) //��������ֽ��
			{
			   switch(BillDataBuffer[1]&0x0F)
			   {
				//case 0x00: BillValue = 1;break; /*����1Ԫ*/
				case 0x01: BillValue = 5;break;
				case 0x02: BillValue = 10;break;
				case 0x03: BillValue = 20;break;
				default:break ;
			  }
				memset(BillDataBuffer,0,sizeof(BillDataBuffer));
			  return BillValue;
		    }
			if(BillStatus ==EscrowRequest)
			{
 			   switch(BillDataBuffer[1]&0x0F)
				{
				   case 0x00: BillValue = 1;break;
				   case 0x01: BillValue = 5;break;
				   case 0x02: BillValue = 10;break;
				   case 0x03: BillValue = 20;break;
				   default:break ;
				}
			/*���ݴ����е�Ǯ��*/
			BillRunStatus =BillInEscrow;            
      }
	  }
	  else if(BillDataBuffer[1]==0x06)
	  {			
		 if(BillDataBuffer[2]==0x09) 

		  BillRunStatus = Normal;
	  }
      else if(BillDataBuffer[1]==0x09)
	  {
		  BillRunStatus = Abnormal;
	  }
		return 0 ;
	}
	else if(BillDataBuffer[0]==0xFF)
	{
		if(CmdNum==1)SetBills(); 
		if(CmdNum==2)DisableBills();
    delay_ms(100);	
  }
	 memset(BillDataBuffer,0,sizeof(BillDataBuffer));
   return 0 ;	
}


   /*******************************************************************************
 * ��������:Polls                                                                     
 * ��    ��:��Ǯ�Ž�Ǯ��                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 *******************************************************************************/

void  Polls(void)
{
//      char i;
//      char Polls_Cmd[2]={0x34,0x01};	
// 	 for(i = 0; i < 2; i++)
// 	 {
// 	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
//         USART_SendData(UART4, Polls_Cmd[i]);//����1����һ���ַ�
// 	 }
}

    /*******************************************************************************
 * ��������:Polls                                                                     
 * ��    ��:��Ǯ�˻ظ��û�                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 *******************************************************************************/

void  BackPolls(void)
{
//      char i;
//      char Polls_Cmd[2]={0x34,0x00};	
// 	 for(i = 0; i < 2; i++)
// 	 {
// 	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
//         USART_SendData(UART4, Polls_Cmd[i]);//����1����һ���ַ�
// 	 }
}

  /*******************************************************************************
 * ��������:ReadBills                                                                     
 * ��    ��:���ý���10  20 5  ��Ǯ                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 *******************************************************************************/
uint8_t  ReadBills(void)
{
    uint8_t CurrentPoint = 0;
	uint8_t CoinsCnt=0;/*Ӳ�Ҽ���*/
 	CurrentPoint=ReadBill();
/*�����������ã���Ϊ�ǹ������ݣ��������ֻ���жϽ��иı䣬��Ӱ����
  �ݵ������ԣ��жϻ��ϳ�������У�����ͬʱ���� */	                          
	//CoinsCnt=UserAct.PayForCoins; /*��Ӳ���������*/
	//UserAct.PayForCoins=0;       /*����Ӳ����*/
	switch(CurrentPoint)
	{
	   case 0:
	           return 0 ;
	   		 
	   case 1  :	  //1Ԫ
	   case 5  : 	  //5Ԫ
	   case 10 :      //10Ԫ
	   case 20 :      //20Ԫ		//��Ǯ��
	              //Polls();
				  Rev_Money_Flag = 1 ;
	              return(CurrentPoint);
  	   default : break;
	}
   return 0 ;
}





