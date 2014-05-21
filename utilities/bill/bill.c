#include "stm32f4xx.h"										 
#include "bsp.h"

 /*******************************************************************************
 * ��������:SetBills                                                                     
 * ��    ��:���ý���10  20 5  ��Ǯ                                                                  
 *           ���ý��յ��ݴ������ڳ����������ѡ���Ǯ����Ǯ                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void SetBills(void)
{
  unsigned char bufCmd[5] = {0x34, 0x00, 0x1e, 0x00, 0x00};
	uint8_t i;
	for(i = 0; i < 5; i++)
	{	
	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
	    USART_SendData(UART4, bufCmd[i]);//����1����һ���ַ�	
	}
}
 /*******************************************************************************
 * ��������:DisableBills                                                                     
 * ��    ��:���ò�����Ǯ                                                               
 *           ���ý��յ��ݴ������ڳ����������ѡ���Ǯ����Ǯ                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void DisableBills(void)
{
  unsigned char bufCmd[5] = {0x34, 0x00, 0x00, 0x00, 0x00};
	uint8_t i;
	for(i = 0; i < 5; i++)
	{	
	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
	    USART_SendData(UART4, bufCmd[i]);//����1����һ���ַ�	
	}
}

/*******************************************************************************/
#define Normal 1 //ֽ�һ���ʼ���ɹ�	30 06 09
#define Abnormal 2  //ֽ�һ����ڹر�״̬ 30 09
#define Erro 3     //ֽ�һ����ص���0xff
#define BillInEscrow 4 //Ǯ���ݴ�����

#define		BillStacked  1
#define		EscrowRequest  2
#define		BillReturned  3
#define		DisabledBillRejected  4	

char BillStatus= 0; //ֽ�һ�״̬
char BillRunStatus;	  //ֽ�һ�����״̬
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
			else if(BillStatus ==EscrowRequest)
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
			else if(BillStatus ==DisabledBillRejected)
			{
				AbnormalHandle(billset_erro);
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
		return nack_flag;
  }
  else if(BillDataBuffer[0]==0x00)
	{
		return ack_flag;
	}
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
unsigned char  Rev_Money_Flag ; 
uint8_t  ReadBills(void)
{
  uint8_t temp = 0;
 	temp=ReadBill();
	memset(BillDataBuffer,0,sizeof(BillDataBuffer));
/*�����������ã���Ϊ�ǹ������ݣ��������ֻ���жϽ��иı䣬��Ӱ����
  �ݵ������ԣ��жϻ��ϳ�������У�����ͬʱ���� */	                          
	switch(temp)
	{
	   case 0:
	           return 0 ;
	   		 
	   case 1  :	  //1Ԫ
	   case 5  : 	  //5Ԫ
	   case 10 :      //10Ԫ
	   case 20 :      //20Ԫ		
	              //Polls(); //��Ǯ��
		  Rev_Money_Flag = 1 ;
	    return(temp);
  	  default : break;
	}
   return 0 ;
}

  /*******************************************************************************
 * ��������:StringToHexGroup                                                                    
 * ��    ��:���ַ�����ת��Ϊhex����,���������޸�                                                                  
 *                                                                               
 * ��    ��:                                                                   
 * ��    ��:bool                                                                      
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 *******************************************************************************/
bool StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
{
  unsigned int i, k=0;
  unsigned char HByte,LByte;
	if(InStrBuffer[strLength-1]!=0x0A)
		return false;
  for(i=0; i<strLength; i=i+3)
  {
    if(InStrBuffer[i]>='0' && InStrBuffer[i]<='9')
    {
      HByte=InStrBuffer[i]-'0';
    }
    else if(InStrBuffer[i]>='A' && InStrBuffer[i]<='F')
    {
      HByte=InStrBuffer[i]-'A' +10;
    }
    else
    {
      HByte=InStrBuffer[i];
      return false;
    }
    HByte=HByte <<4;
    HByte = HByte & 0xF0;
    if(InStrBuffer[i+1]>='0' && InStrBuffer[i+1]<='9')
    {
       LByte=InStrBuffer[i+1]-'0';
    }
    else if(InStrBuffer[i+1]>='A' && InStrBuffer[i+1]<='F')
    {
       LByte=InStrBuffer[i+1]-'A' +10;
    }
    else
    {
       LByte=InStrBuffer[i];
       return false;
    }
		if(InStrBuffer[i+2]==0x20)
		{			
			 OutHexBuffer[k++]=HByte |LByte;
		}
		if(InStrBuffer[i+2]==0x0D)
		{
			OutHexBuffer[k++]=HByte |LByte;
			return true ;
		}
 }
 return true;
}

