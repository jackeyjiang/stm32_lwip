#include"bsp.h"

 uint8_t PrintBuf[10];
 uint8_t PrintIndex;

 /*******************************************************************************
 * ��������:Uart1_Card                                                                      
 * ��    ��:����ͨ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
void Uart1_Card(uint8_t *p,uint8_t sizeData)
{
      uint8_t i;		   
	  for(i=0; i<sizeData; i++)
	  {
        USART_SendData(USART1, p[i]);//����1����һ���ַ�
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ��������
 	  }
}								   	
										
 unsigned char PrintInitCmd[]={0x1b,0x40}; //��ӡ����ʼ��
 unsigned char SetEntryPrintCHinese[]={0x1c,0x26};	  //���뺺�Ӵ�ӡģʽ
 const unsigned char SetExitPrintCHinese[]={0x1c,0x2e};	  //�˳���ӡģʽ
 unsigned char PrintMessage0[]    ={" ���ڷ�Ī˹�������ܲ������޹�˾"};	  //�˳���ӡģʽ
 unsigned char PrintMessage1[]    ={"��Ʒ     ����     ����     ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename0[]={"���ܲ�����   ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename1[]={"�㹽����     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename2[]={"��Ƥ��Ѽ     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename3[]={"�������     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessage2[]    ={"ˢ����             �ֽ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage3[]    ={"Ӧ�գ�             �һأ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage4[]    ={"ˢ����             Ӧ�գ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage5[]    ={"     лл�ݹˣ��뱣������СƱ   "};	  //�˳���ӡģʽ
 unsigned char PrintMessage6[]    ={"          ��ӭ�´λݹ�          "};	  //�˳���ӡģʽ
 unsigned char PrintMessage7[]    ={"   ʱ�䣺2013.08.12 12:30       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage8[]    ={"   �绰��0755-XXXXXXXXX         "};	  //�˳���ӡģʽ


 /*******************************************************************************
 * ��������:CheckPrintStatus                                                                     
 * ��    ��:����ӡ����״̬�Ƿ���ֽ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
uint8_t CheckPrintStatus(void)
{
  uint8_t Cmd[]={0x10,0x04,0x04};
  uint8_t CurrentPoint = 0;
  switch(CurrentPoint)
  {
    case 0:
	  {			
			PrintIndex = 0;
	    Uart1_Card(Cmd,sizeof(Cmd));
	    CurrentPoint = 1;
			delay_ms(100);
	  }break;	  
	  case 1: 
	  {
	    switch(PrintBuf[0])
		  {
			  case 0x00 :	break ;
			  case 0x02 :	break ;
			  case 0x0c :	break ;
			  case 0x10 :	break ;
			  case 0x60 :	return 1;     //break ;  //��ʾû��ֽ��
				default: break;
		  }
    }  
    return 0;//��ʾ����
  }
	return 1;
}
 /*******************************************************************************
 * ��������:COPY                                                                    
 * ��    ��:��ȡʱ���Ǯ������������Ϊ��ӡ������                                                                  
 *                                                                               
 * ��    ��: a , *p0 , *p1                                                                   
 * ��    ��:��                                                                     
 * ��    ��:��                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
void COPY(Struct_TD  a,unsigned char *p0,unsigned char *p1)
{										
	p0[5]=UserAct.PayShould/100+'0';
	p0[6]=UserAct.PayShould%100/10+'0';
	p0[7]=UserAct.PayShould%100%10+'0';

	p0[13]=Print_Struct.P_paymoney /100+'0';
	p0[14]=Print_Struct.P_paymoney %100/10+'0';
	p0[15]=Print_Struct.P_paymoney %100%10+'0';

	p0[21]=Print_Struct.P_MoneyBack/100+'0';
	p0[22]=Print_Struct.P_MoneyBack%100/10+'0';
	p0[23]=Print_Struct.P_MoneyBack%100%10+'0';
			
	p1[5]  ='2';
	p1[6] = '0';
	p1[7] = a.Year     / 10+'0';
	p1[8] = a.Year     % 10+'0';
	p1[10] = a.Month    / 10+'0';
	p1[11] = a.Month    % 10+'0';
	p1[13] = a.Date     / 10+'0';
	p1[14] = a.Date     % 10+'0';
	p1[16] = a.Hours    / 10+'0';
	p1[17] = a.Hours    % 10+'0';
	p1[19] = a.Minutes  / 10+'0';
	p1[20] = a.Minutes  % 10+'0';
	p1[22] = a.Senconds / 10+'0';
  p1[23] = a.Senconds % 10+'0';
}

 /*******************************************************************************
 * ��������:CheckPrintStatus                                                                     
 * ��    ��:����ӡ����״̬�Ƿ���ֽ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2013��8��28�� 


 ��Ʒ       ����  ����  ���ܼ�
���ܲ�����    1    15     15
�㹽����      1    20     20
�������      1    25     25
��Ƥ��Ѽ      1    30     30
���ʽ�����п�֧��/����֧ͨ��/�ֽ�֧��
Ӧ��:90  ����:100  �һ�:10
ʱ��:2014-04-16 23:19

 *******************************************************************************/


 P_stuction Print_Struct;
void  SPRT(void)
{
  unsigned char   SendStc[3]={0x1d,0x56,0x01};		//��ֽ����
	unsigned char  huan3[]={0x1b,0x64,0x06};
	unsigned char  p0[]={"Ӧ��:201����:   �һ�:   \r\n"};
	unsigned char  p1[]={"ʱ��:2013-12-15-12:30:00\r\n"};
	unsigned char  p2[]={"���ܲ�����1\t15\t015\r\n"};
	unsigned char  p3[]={"�㹽����  1\t15\t015\r\n"};
	unsigned char  p4[]={"��Ƥ��Ѽ  1\t15\t015\r\n"};
	unsigned char  p5[]={"�������  1\t15\t015\r\n"};

  printf("\r\n");//��ӡ�س�����
  printf("��Ʒ\t����\t����\t���\r\n"); 	
  RTC_TimeShow();//�õ���ǰ��ʱ��
	COPY(TimeDate,p0,p1);
  if(Print_Struct.P_Number1st>0)
  {	
		p2[10]= Print_Struct.P_Number1st%10 +'0'; 
		p2[15]=UserAct.MealCost_1st/100+'0';
		p2[16]=UserAct.MealCost_1st%100/10+'0';
		p2[17]=UserAct.MealCost_1st%100%10+'0';
		printf("%s",p2);
  }
	if(Print_Struct.P_Number2nd>0)
	{
		p3[10]= Print_Struct.P_Number2nd%10 +'0'; 
		p3[15]=UserAct.MealCost_2nd/100+'0';
		p3[16]=UserAct.MealCost_2nd%100/10+'0';
		p3[17]=UserAct.MealCost_2nd%100%10+'0';
		printf("%s",p3);
	}
  if(Print_Struct.P_Number3rd>0)
	{
		p4[10]= Print_Struct.P_Number3rd%10 +'0'; 
		p4[15]=UserAct.MealCost_3rd/100+'0';
		p4[16]=UserAct.MealCost_3rd%100/10+'0';
		p4[17]=UserAct.MealCost_3rd%100%10+'0';
		printf("%s",p4);
	}
	if(Print_Struct.P_Number4th>0)
	{		
		p5[10]= Print_Struct.P_Number4th%10 +'0'; 
		p5[15]=UserAct.MealCost_4th/100+'0';
		p5[16]=UserAct.MealCost_4th%100/10+'0';
		p5[17]=UserAct.MealCost_4th%100%10+'0';
		printf("%s",p5);
	} 
  printf("%s",p0);
	printf("%s",p1);
	if(UserAct.PayType == '2' )
	{
		 printf("֧����ʽ�����п�֧��\r\n");
	}
  if(UserAct.PayType == '1')
	{
		 printf("֧����ʽ���ֽ�֧��\r\n");
	}
	if(UserAct.PayType == '3')
	{
		printf("֧����ʽ������֧ͨ��\r\n");
	}
	Uart1_Card(huan3,sizeof(huan3)); 
	Uart1_Card(SendStc,sizeof(SendStc));//	��ֽ
}
