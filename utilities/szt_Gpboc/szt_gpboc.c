#include "stm32f4xx.h"
#include "szt_gpboc.h"
#include "bsp.h"
#include "rtc.h"

sendorderDef SztSendData;
sendorderDef GpbocSendData;
uint8_t MustSendData[21]; //����Ҫ���͵�����
uint8_t  AckOver; //Ack�����Ƿ������
uint8_t  DataOver;//���������Ƿ������
uint8_t  Usart2Flag;//�������ͣ���Ϊ �����ݣ�0��Ack��1��data��2

uint8_t PrintBuf2[640];
uint32_t PrintIndex2;

uint8_t SztSamNum[4]; //����ͨSam����
uint8_t GpbocSamNum[4];//����Sam����
uint8_t UserNo[3] = {0x00,0x01,0x23};//����Ա���,BCD��,3λ,��123��ʾΪ0x00,0x01,0x23
uint8_t newtime[8];//ͬ�� ʱ��ֵ��
uint8_t timeMode;//ͬ����ʽ��0Ϊһ��ͬ����1Ϊǿ��ͬ��
extern Struct_TD TimeDate ;		 //ϵͳʱ�䣬��7Data
uint8_t SztReadCardTime; //����ͨ����ʱ��

/*������ʼ�������������32�������˵�һ����Ҫ���޸��⣬�����Ķ�Ϊ�����֣�ȫΪ0
��һ������ʾ�յ��и�����Ĭ��Ϊ0x00����ʾһ���յ��С�����";
����0ʱ���� 0x01һ���յ��У�������0x02�������յ��У�����&�������ã�
*/
uint8_t Gpboc_Init_PutInParameter[32] ={
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} ;

/* �����ϴ�������32������ΪԤ��ֵ0x00*/
uint8_t Gpboc_SendDataParameter[32] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	
};


/*�������ᡢǩ�˵Ĳ�����32������ΪԤ��ֵ0x00*/
uint8_t Gpboc_CheckOutParameter[32] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	
};

DeviceInf Szt_deviceInf; //����ͨ״̬��ѯ����ֵ

uint8_t TimeResult;	 //ʱ��ͬ���Ľ��
MachineTimeDef MachineTime;	  //��ǰ�豸ʱ��

SztReductInfStr SztReductInf;	//����ͨ�ۿ��ֵ
SztReadCardInfStr SztCardInf;	//����ͨ��������ֵ

GpbocInitInf UpInitData[10];   //����10������ǩ�����ݽṹ��
uint8_t UpInitErrNum[2];	   //�������ش���
uint8_t UpInitGpbocErrNum[2];  //��������
GpbocReadCardInf UpReadCardData;  //������������
TRANSLOG_TO_HOST UpDeductData;	//�����ۿ������
//uint8_t UpDedectBackup[600];  //һ�����ѱ�������
UPLOADLOG_TO_HOS UpSendData;   //�����ϴ����ݷ��ؽṹ��
LOGINLOG_TO_HOST UpCheckOutData; //�������᷵�ؽṹ��

//���׳ɹ������������ﵽҪ������������������ݣ���������֮�������0
uint8_t freq_toSendGpboc;
//���׳ɹ������������ﵽҪ���������������ͨ���ݣ���������֮�������0
uint8_t freq_toSendSzt;


//extern u32 tmp_hh = 0x17, tmp_mm = 0x3b, tmp_ss = 0x01;
//extern u32 tmp_yy = 0x09, tmp_m2 = 0x01, tmp_dd = 0x01,tmp_ww=0x01;
/***************
����2��Ϊ����ͨ������ˢ��ͨ��
****************/



/*������ͨ\����ͨ��*/
void Open_Szt_Gpboc(void)
{
  USART_Cmd(USART2,ENABLE);
  //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/*�ر�����ͨ\����ͨ��*/
void Close_Szt_Gpboc(void)
{
  USART_Cmd(USART2,DISABLE);
  //USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}



/*����ȷӦ��:ACK*/
void return_ACK(void)
{
   uint8_t ackdata[6] = {DataHead,BeginCode1,BeginCode2,0x00,0xff,EndCode};
   uint8_t i;

   for(i=0;i<6;i++)
   {
     USART_SendData(USART2,ackdata[i]);
	 while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);//�ȴ��������
   }
}


/*�ش���Ӧ��:NACK*/
void return_NACK(void)
{
  uint8_t nackdata[6] = {DataHead,BeginCode1,BeginCode2,0xff,0x00,EndCode};
  uint8_t i;

  for(i=0;i<6;i++)
  {
    USART_SendData(USART2,nackdata[i]);
	while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);//�ȴ��������
  }
}




/*����ͨ�̶��õ������ݣ��������Ϊ��saddr��ʼ��Dcs֮ǰ���е����ݳ��ȣ��Լ���������*/
void SztFixData(uint8_t LenH,uint8_t LenL,uint8_t OrderNum)
{
  SztSendData.datahead = DataHead;
  SztSendData.startPacket[0] = BeginCode1;
  SztSendData.startPacket[1] = BeginCode2;
  SztSendData.dataLen[0] = LenH;
  SztSendData.dataLen[1] = LenL;
  SztSendData.saddr[0] = 0x06;
  SztSendData.saddr[1] = 0xff;
  SztSendData.saddr[2] = 0xff;
  SztSendData.saddr[3] = 0xff;
  SztSendData.saddr[4] = 0xff;
  SztSendData.taddr[0] = 0x05;
  SztSendData.taddr[1] = SztSamNum[0];
  SztSendData.taddr[2] = SztSamNum[1];
  SztSendData.taddr[3] = SztSamNum[2];
  SztSendData.taddr[4] = SztSamNum[3];
  SztSendData.startNum ++ ; //�ٸĹ�
  SztSendData.SendCommand = OrderNum;
  SztSendData.plen1[0] = 0x00;
  SztSendData.plen1[1] = 0x01;
  SztSendData.plen2[0] = 0x00;
  SztSendData.plen2[1] = 0x01;
  SztSendData.dataend = 0x00;

  MustSendData[0] = SztSendData.datahead ;
  MustSendData[1] =   SztSendData.startPacket[0];
  MustSendData[2] =   SztSendData.startPacket[1];
  MustSendData[3] =   SztSendData.dataLen[0];
  MustSendData[4] =   SztSendData.dataLen[1];
  MustSendData[5] =   SztSendData.saddr[0];
  MustSendData[6] =   SztSendData.saddr[1];
  MustSendData[7] =   SztSendData.saddr[2];
  MustSendData[8] =   SztSendData.saddr[3];
  MustSendData[9] =   SztSendData.saddr[4];
  MustSendData[10] =   SztSendData.taddr[0];
  MustSendData[11] =   SztSendData.taddr[1];
  MustSendData[12] =   SztSendData.taddr[2];
  MustSendData[13] =   SztSendData.taddr[3];
  MustSendData[14] =   SztSendData.taddr[4];
  MustSendData[15] =   SztSendData.startNum;
  MustSendData[16] =   SztSendData.SendCommand;
  MustSendData[17] =   SztSendData.plen1[0];
  MustSendData[18] =   SztSendData.plen1[1];
  MustSendData[19] =   SztSendData.plen2[0];
  MustSendData[20] =   SztSendData.plen2[1];
}




/* �����̶��õ������ݣ��������Ϊ��saddr��ʼ��Dcs֮ǰ���е����ݳ��ȣ��Լ���������,
�Ա�����ͨ������û��Sam����taddrȫΪ0xff������Sam����taddr����Sam����*/
void GpbocFixData(uint8_t LenH,uint8_t LenL,uint8_t OrderNum)
{
  GpbocSendData.datahead = DataHead;
  GpbocSendData.startPacket[0] = BeginCode1;
  GpbocSendData.startPacket[1] = BeginCode2;
  GpbocSendData.dataLen[0] = LenH;
  GpbocSendData.dataLen[1] = LenL;
  GpbocSendData.saddr[0] = 0x06;
  GpbocSendData.saddr[1] = 0xff;
  GpbocSendData.saddr[2] = 0xff;
  GpbocSendData.saddr[3] = 0xff;
  GpbocSendData.saddr[4] = 0xff;
  GpbocSendData.taddr[0] = 0x05;
  GpbocSendData.taddr[1] = GpbocSamNum[0];
  GpbocSendData.taddr[2] = GpbocSamNum[1];
  GpbocSendData.taddr[3] = GpbocSamNum[2];
  GpbocSendData.taddr[4] = GpbocSamNum[3];
  SztSendData.startNum ++;//�ٸĹ�
  GpbocSendData.SendCommand = OrderNum;
  GpbocSendData.plen1[0] = 0x00;
  GpbocSendData.plen1[1] = 0x01;
  GpbocSendData.plen2[0] = 0x00;
  GpbocSendData.plen2[1] = 0x01;
  GpbocSendData.dataend = 0x00;

  MustSendData[0] = GpbocSendData.datahead ;
  MustSendData[1] =   GpbocSendData.startPacket[0];
  MustSendData[2] =   GpbocSendData.startPacket[1];
  MustSendData[3] =   GpbocSendData.dataLen[0];
  MustSendData[4] =   GpbocSendData.dataLen[1];
  MustSendData[5] =   GpbocSendData.saddr[0];
  MustSendData[6] =   GpbocSendData.saddr[1];
  MustSendData[7] =   GpbocSendData.saddr[2];
  MustSendData[8] =   GpbocSendData.saddr[3];
  MustSendData[9] =   GpbocSendData.saddr[4];
  MustSendData[10] =   GpbocSendData.taddr[0];
  MustSendData[11] =   GpbocSendData.taddr[1];
  MustSendData[12] =   GpbocSendData.taddr[2];
  MustSendData[13] =   GpbocSendData.taddr[3];
  MustSendData[14] =   GpbocSendData.taddr[4];
  MustSendData[15] =   SztSendData.startNum;
  MustSendData[16] =   GpbocSendData.SendCommand;
  MustSendData[17] =   GpbocSendData.plen1[0];
  MustSendData[18] =   GpbocSendData.plen1[1];
  MustSendData[19] =   GpbocSendData.plen2[0];
  MustSendData[20] =   GpbocSendData.plen2[1];
}


/*�жϽ��յ�������ACK����NACK*/
uint8_t ManageACK(void)
{
  uint8_t i;
  if(( PrintBuf2[0] == 0x00) && ( PrintBuf2[1] == 0x00) &&( PrintBuf2[2] == 0xff ) && (PrintBuf2[3] == 0x00 ) && (PrintBuf2[4] ==0xff ) && (PrintBuf2[5] ==0x00 ))
  {
    return IsAck;
  }
  else if(( PrintBuf2[0] == 0x00) && ( PrintBuf2[1] == 0x00) &&( PrintBuf2[2] == 0xff ) && (PrintBuf2[3] == 0xff ) && (PrintBuf2[4] ==0x00 ) && (PrintBuf2[5] ==0x00 ))
  {
    return IsNack;
  }
  else
  {
    //printf("\ndata is :");
	for(i=0;i<6;i++)
	{
	  //printf(" %x ",PrintBuf2[i]); 
	}
    return IsAckError;
  }

}


/*�ԱȽ��յ������ݣ��ж����Ƿ�����*/
uint8_t ManageRxData(void)
{
   uint16_t i;
   uint8_t sumdata =0;

   /*
   for( i=0;i<((UsartBuff[9]<< 8) + UsartBuff[10] +7);i++)
   {
     //printf(" %x ",UsartBuff[i]);  //��ӡ���н��յ�������
   }
   */
   for( i=0;i<((PrintBuf2[9]<< 8) + PrintBuf2[10] +1);i++)
   {
     sumdata = sumdata + PrintBuf2[11+i]; //�ӵ�12������ʼ��ӣ�һֱ�ӵ�У��ֵ
   }

   if(( sumdata == 0 ) && ( PrintBuf2[6] == 0x00) && (PrintBuf2[7] == 0x00) &&(PrintBuf2[8] == 0xff ) && (PrintBuf2[PrintIndex2 - 1] == 0x00))
   {
     return_ACK();
	 return DataReOk;
   }
   else
   {
     return_NACK();
	 return DataReErr;
   }
}

uint8_t ManageReOrderType(void)
{
  uint32_t temp; 
  uint8_t  ackflagbuff = IsNc;
  uint8_t  dataflagbuff = DataNc;
  uint8_t  orderType = 0 ;
  //Open_Szt_Gpboc();
  delay_us(5);  
  //Order_Ifn_QueryStatus(); 
  while(1) 
  {
    if((Usart2Flag == AckFlag) || (AckOver == RxOk))
       break;
	  temp ++;
	  delay_us(50);
	  if(temp >= 0x0fff0 )
			break;;
  }

  //printf("\n????Ack????:%d \n",temp );

  temp =0;
  while(1)
  {
    if( AckOver == RxOk )
	    break;
	  temp ++;
	  delay_us(50);
	  if(temp >= 0x0fff0)
	    break;  
	}
  ackflagbuff = ManageACK();
  if(ackflagbuff == IsNc)
  {
    //printf("\n?????\n");
  }
  else if(ackflagbuff == IsAck)
  {
    //printf("\n??????\n");
  }
  else if( ackflagbuff == IsNack)
  {
     //printf("\n???????\n");
  }
  else
  {
     //printf("\n??????,ack????:%d\n",ackflagbuff);
  }
  if( ackflagbuff == IsAck )
  {  
	  temp =0;
		while(1) 
		{
		  if( Usart2Flag == DataFlag )
			  break;
		  temp ++;
		  delay_us(50);
		  if( temp >= 0x0fff0 )
			  break;
		}
		temp =0;
		while(1)
		{
		  if(DataOver == RxOk)
			  break;
		  temp ++;
		  delay_us(50);
		  if(temp >= 0x0fff0)
			  break;
		}	  
		dataflagbuff = ManageRxData();
	  
		if( dataflagbuff == DataNc)
		{
		  //printf("\n????????\n");
		}
		else if( dataflagbuff == DataReOk)
		{
		   //printf("\n??????\n");   
		   orderType = PrintBuf2[22];  
		}
		else if( dataflagbuff == DataReErr)
		{
		   //printf("\n???????\n");
		}
		else 
		{
		   //printf("\n??????,????????%d\n",dataflagbuff);
		}
  	}
  //printf("\nPrintIndex2= %d",PrintIndex2);
	//printf("\n??????:%x",orderType);
  
  //Close_Szt_Gpboc();

  return orderType;
}








/*�����豸״̬��ѯ����*/
void Order_Ifn_QueryStatus(void)
{
  uint8_t data[23];
  uint8_t i;
  uint8_t temp=0;

  SztSamNum[0] = 0;
  SztSamNum[1] = 0;
  SztSamNum[2] = 0;
  SztSamNum[3] = 0;
  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x10,T_QueryStatus); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }
  
  for(i=0;i<16;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[21] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[22] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<23;i++)			 //�����豸״̬��ѯ����
  {
    USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<23;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}






/*��������ͨ����Ա��¼����*/
void Order_SztUserLogin(void)
{
  uint8_t data[26];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x13,T_UserLogin); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  for(i=0;i<3;i++)
	{
	  data[21+i] = UserNo[i];
	}
  
  for(i=0;i<19;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[24] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[25] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<26;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<26;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}






/*��������ͨʱ��ͬ������*/
void Order_SztTimeCheck(void)
{
  uint8_t data[32];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x19,T_TimeCheck); //����Ҫ���͵�����
  
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }
  /*
  for(i=0;i<8;i++)
  	{
  	  data[21+i] = newtime[i];
  	}
	*/
	RTC_TimeShow();

  data[21] = Year_Frist;
  data[22] = DectoBCD(TimeDate.Year);
  data[23] = DectoBCD(TimeDate.Month);
  data[24] = DectoBCD(TimeDate.Date);
  data[25] = DectoBCD(TimeDate.Hours);
  data[26] = DectoBCD(TimeDate.Minutes);
  data[27] = DectoBCD(TimeDate.Senconds);
  data[28] = DectoBCD(TimeDate.WeekDay);

  data[29] = timeMode;  //ʱ��ͬ����ʽ
  //printf("\nϵͳʱ��Ϊ:%x%x-%x-%x %x:%x:%x",data[21],data[22],data[23],data[24],data[25],data[26],data[27],data[28]);
  for(i=0;i<25;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[30] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[31] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<32;i++)			 //��������ͨʱ��ͬ������
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<32;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}






/*��������ͨ�״οۿ�����������Ϊ�ۿ���*/
void Order_SztDeductOnce(int32_t money)
{
  uint8_t data[34];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x1B,T_SztDeductOnce); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  data[21] = money;
  data[22] = money>>8;
  data[23] = money>>16;
  data[24] = money>>24;

  /*

  for(i=0;i<7;i++)  //�ۿ�ʱ��
  	{
  	  data[25+i] = newtime[i];
  	}
  */
  //�ۿ�ʱ��
  RTC_TimeShow();
  data[25] = Year_Frist;;
  data[26] = DectoBCD(TimeDate.Year);
  data[27] = DectoBCD(TimeDate.Month);
  data[28] = DectoBCD(TimeDate.Date);
  data[29] = DectoBCD(TimeDate.Hours);
  data[30] = DectoBCD(TimeDate.Minutes);
  data[31] = DectoBCD(TimeDate.Senconds);
  
  //printf("\n�ۿ�ʱ��Ϊ:%x%x-%x-%x %x:%x:%x",data[25],data[26],data[27],data[28],data[29],data[30],data[31]);

  for(i=0;i<27;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[32] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[33] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<34;i++)			 //��������ͨʱ��ͬ������
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<34;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}





/*��������ͨ�ٴοۿ�����������Ϊ�ۿ���*/
void Order_SztDeductAgain(int32_t money)
{
  uint8_t data[34];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x1B,T_SztDeductAgain); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  data[21] = money;
  data[22] = money>>8;
  data[23] = money>>16;
  data[24] = money>>24;
  
  /*
  for(i=0;i<7;i++)  //�ۿ�ʱ��
  	{
  	  data[25+i] = newtime[i];
  	}
  	*/
  //�ۿ�ʱ��
  RTC_TimeShow();
  data[25] = Year_Frist;;
  data[26] = DectoBCD(TimeDate.Year);
  data[27] = DectoBCD(TimeDate.Month);
  data[28] = DectoBCD(TimeDate.Date);
  data[29] = DectoBCD(TimeDate.Hours);
  data[30] = DectoBCD(TimeDate.Minutes);
  data[31] = DectoBCD(TimeDate.Senconds);
  
  //printf("\n�ۿ�ʱ��Ϊ:%x%x-%x-%x %x:%x:%x",data[25],data[26],data[27],data[28],data[29],data[30],data[31]);

  for(i=0;i<27;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[32] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[33] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<34;i++)			 //��������ͨʱ��ͬ������
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<34;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}





/*��������ͨ�״ζ�������*/
void Order_SztReadCardOnce(void)
{
  uint8_t data[24];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x11,T_SztReadCardOnce); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  
  data[21] = SztReadCardTime;
  	
  
  for(i=0;i<17;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[22] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[23] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  

  for(i=0;i<24;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<24;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}



/*��������ͨ�ٴζ�������*/
void Order_SztReadCardAgain(void)
{
  uint8_t data[24];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x11,T_SztReadCardAgain); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  
  data[21] = SztReadCardTime;
  	
  
  for(i=0;i<17;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[22] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[23] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<24;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<24;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}




/*��������ͨ�Ͽ���������*/
void Order_SztUnlink(void)
{
  uint8_t data[23];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x10,T_SztUnlink); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }
  
  for(i=0;i<16;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[21] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[22] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<23;i++)			 //�����豸״̬��ѯ����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<23;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}



/*����ͨ����ǩ����������������Ϊ0x03*/
void Order_SztAutoCheckIn(void)
{
  uint8_t data[24];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x11,T_SztCheck_In); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  data[21] = 0x03;
  
  for(i=0;i<17;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[22] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[23] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<24;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<24;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}




/*����ͨ������������������������Ϊ0x01*/
void Order_SztAutoDownload(void)
{
  uint8_t data[24];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x11,T_SztCheck_In); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  data[21] = 0x01;
  
  for(i=0;i<17;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[22] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[23] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<24;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<24;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}



/*����ͨ�����ϴ�����������������Ϊ0x02*/
void Order_SztAutoSendData(void)
{
  uint8_t data[24];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x11,T_SztCheck_In); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  data[21] = 0x02;
  
  for(i=0;i<17;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[22] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[23] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<24;i++)			 //��������ͨ����Ա��¼����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<24;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}






/*����ͨ��ֵ�����״�����*/
void Order_SztRechargeOnce(void)
{
  uint8_t data[30];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x17,T_SztRechargeOnce); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }

  for(i=0;i<7;i++)
  	{
  	  data[21+i] = newtime[i];
  	}
  
  for(i=0;i<23;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }

  data[28] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[29] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<30;i++)			 //�����豸״̬��ѯ����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<30;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}



/*����ͨ��ֵ�����ٴ�����*/
void Order_SztRechargeAgain(void)
{
  uint8_t data[30];
  uint8_t i;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

 
  delay_us(5);
  
  SztFixData(0x00,0x17,T_SztRechargeAgain); //����Ҫ���͵�����
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];		  //�ѱ���Ҫ���͵�����д��������
  }
  
  for(i=0;i<16;i++)			 //����ҪУ������ݵĺ�
  {
    temp = temp + data[5+i];
  }
  
  for(i=0;i<7;i++)
  	{
  	  data[21+i] = newtime[i];
  	}

  data[28] = 0 - temp;		   //������룬����+ҪУ������ݵĺ� = 0
  data[29] = SztSendData.dataend;	   //������
  //printf(" %x ",data[21]);
  
  

  for(i=0;i<30;i++)			 //�����豸״̬��ѯ����
  {
    //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  /*
  for(i=0;i<30;i++)
  {
     printf(" %x ",data[i]);
  }
  */
}






/*������ʼ��ǩ������*/
void Order_Gpboc_Init(void)
{
  uint8_t data[55];	//48�����ȣ�����32��Ϊ���ݲ���,��0��20Ϊ�̶�����21��52Ϊ32�����������53ΪУ���룬54Ϊ������־
  uint8_t i;
  //sendorderDef senddata;
  uint8_t temp=0;


  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;
  GpbocSamNum[0] = 0xff;
  GpbocSamNum[1] = 0xff;
  GpbocSamNum[2] = 0xff;
  GpbocSamNum[3] = 0xff;
  
  delay_us(5);

  GpbocFixData(0x00,0x30,T_GpbocCheck_In); 
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];
  }
  
  for( i=0;i<32;i++)
  {
    data[i+21] = Gpboc_Init_PutInParameter[i];
  }

  for(i=0;i<48;i++)
  {
    temp = temp + data[5+i];
  }
 // senddata.Dcs= 0 - temp;
  data[53] = 0 - temp;//senddata.Dcs;
  data[54] = SztSendData.dataend;
  //printf("У����Ϊ�� %x \n",data[53]);
  /*
  for(i=0;i<55;i++)
  	{
  	  printf(" %x ",data[i]);
  	}
	*/

  for(i=0;i<55;i++)
  {
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
} 



 
/*��ȡ�������ڵ��������,������Ϊ����ͨ\�������Ͽ�*/
void Order_Gpboc_ReadCard (void)
{
  uint8_t data[25];	//18�����ȣ�����2��Ϊ���ݲ���,��0��20Ϊ�̶�����21��22Ϊ2�����������23ΪУ���룬24Ϊ������־
  uint8_t i;
  sendorderDef senddata;
  uint8_t temp=0;

 
  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;

  delay_us(5);

  GpbocFixData(0x00,0x12,T_GpbocQueryReadCard); 
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];
  }
  
  data[21] = 0x02;//0x01Ϊֻ�����п���0x02Ϊһ�ſ���������ͨ������������
  data[22] = 0x00;

  for(i=0;i<18;i++)
  {
    temp = temp + data[5+i];
  }
  senddata.Dcs= 0 - temp;
  data[23] = senddata.Dcs;
  data[24] = SztSendData.dataend;
  //printf("У����Ϊ�� %x \n",data[23]);
  /*
  for(i=0;i<25;i++)
  	{
  	  printf(" %x ",data[i]);
  	}
*/

  for(i=0;i<25;i++)
  {
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
}




/*�������ۿ�����*/
void Order_Gpboc_Deduct(uint32_t submoney)
{
  uint8_t data[28];	//21�����ȣ�����5��Ϊ���ݲ���,��0��20Ϊ�̶�����21��25Ϊ32�����������26ΪУ���룬27Ϊ������־
  uint8_t i;
  sendorderDef senddata;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;
  
  delay_us(5);

  GpbocFixData(0x00,0x15,T_GpbocDeduct); 
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];
  }

  
  data[21] = submoney;//0x01;
  data[22] = submoney >>8;//0x00;
  data[23] = submoney >>16;//0x00;
  data[24] = submoney >>24; //0x00; 
  /*
  data[21] =0x01;
  data[22] =0x00;
  data[23] =0x00;
  data[24] =0x00; 
  */
  data[25] = 0x20;  // Ѱ��ʱ�䣬Ϊ0x00ʱ���鿨15�룻Ϊ����ֵʱ���ֱ�Ϊ���������ֵ
  
  for(i=0;i<21;i++)
  {
    temp = temp + data[5+i];
  }
  senddata.Dcs= 0 - temp;
  data[26] = senddata.Dcs;
  data[27] = SztSendData.dataend;
  //printf("У����Ϊ�� %x \n",data[26]);
  /*
  for(i=0;i<28;i++)
  	{
  	  printf(" %x ",data[i]);
  	}
	*/

  for(i=0;i<28;i++)
  {
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
}




/*�����ϴ���������*/
void Order_Gpboc_SendData(void)
{
  uint8_t data[55];	//48�����ȣ�����32��Ϊ���ݲ���,��0��20Ϊ�̶�����21��52Ϊ32�����������53ΪУ���룬54Ϊ������־
  uint8_t i;
  //sendorderDef senddata;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;
  
  delay_us(5);

  GpbocFixData(0x00,0x30,T_GpbocSendData); 
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];
  }
  
  for( i=0;i<32;i++)
  {
    data[i+21] = Gpboc_SendDataParameter[i];
  }

  for(i=0;i<48;i++)
  {
    temp = temp + data[5+i];
  }
 // senddata.Dcs= 0 - temp;
  data[53] = 0 - temp;//senddata.Dcs;
  data[54] = SztSendData.dataend;
  //printf("У����Ϊ�� %x \n",data[53]);
  /*
  for(i=0;i<55;i++)
  	{
  	  printf(" %x ",data[i]);
  	}
	*/

  for(i=0;i<55;i++)
  {
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
}






/* ���������㡢ǩ������*/
void Order_Gpboc_Check_Out(void)
{
  uint8_t data[55];	//48�����ȣ�����32��Ϊ���ݲ���,��0��20Ϊ�̶�����21��52Ϊ32�����������53ΪУ���룬54Ϊ������־
  uint8_t i;
  sendorderDef senddata;
  uint8_t temp=0;

  PrintIndex2 = 0;
  memset(PrintBuf2,0xff,640);
  Usart2Flag = NormalFlag;
  DataOver = RxNo;
  AckOver = RxNo;
  
  delay_us(5);

  GpbocFixData(0x00,0x30,T_GpbocCheck_Out); 
  for( i=0;i<21;i++)
  {
    data[i] = MustSendData[i];
  }
  
  for( i=0;i<32;i++)
  {
    data[i+21] = Gpboc_CheckOutParameter[i];
  }

  for(i=0;i<48;i++)
  {
    temp = temp + data[5+i];
  }
  senddata.Dcs= 0 - temp;
  data[53] = senddata.Dcs;
  data[54] = SztSendData.dataend;
  //printf("У����Ϊ�� %x \n",data[53]);
  /*
  for(i=0;i<55;i++)
  	{
  	  printf(" %x ",data[i]);
  	}
	*/

  for(i=0;i<55;i++)
  {
    USART_SendData(USART2,data[i]);
  	while( USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
}





/*�����豸״̬��ѯ,����1�ɹ�������0ʧ��*/
uint8_t ManageQueryStatus(void)
{
   uint8_t i;
    
   if( PrintBuf2[27] == 0x00)	 //����
   {
      //printf("\n״̬��ѯ�ɹ�\n");

	  SztSamNum[0] = PrintBuf2[12];
	  SztSamNum[1] = PrintBuf2[13];
	  SztSamNum[2] = PrintBuf2[14];
	  SztSamNum[3] = PrintBuf2[15];
	  
      for(i=0;i<4;i++)
	  {
	    Szt_deviceInf.Device_ID[i] = PrintBuf2[28 +i];       
	  }
	   //ֱ��16�������
	  //printf("\nSAMIDΪ:%x%x%x%x",Szt_deviceInf.Device_ID[0],Szt_deviceInf.Device_ID[1],Szt_deviceInf.Device_ID[2],Szt_deviceInf.Device_ID[3]); 


	  Szt_deviceInf.Terminal_ID = PrintBuf2[32] + (PrintBuf2[33]<<8) + (PrintBuf2[34]<<16) + (PrintBuf2[35]<<24);
	  
	  //printf("\n����ͨ�ն˱��Ϊ��%d\n",Szt_deviceInf.Terminal_ID);


	  for(i=0;i<10;i++)
	  {
	    Szt_deviceInf.SoftWare_Ver[i] = PrintBuf2[36+i];
	  }
	  /*
	  printf("\n�豸����汾��Ϊ��");
	  for(i=0;i<10;i++)
	  {
	    printf("%c",Szt_deviceInf.SoftWare_Ver[i]);
	  }
		*/

	  for(i=0;i<4;i++)
	  {
	     Szt_deviceInf.Device_Status[i] = PrintBuf2[46 +i];
	  }
	  //�豸״̬��

	  
	  Szt_deviceInf.Curr_RemainMoney = PrintBuf2[50] + (PrintBuf2[51]<<8) + (PrintBuf2[52]<<16) +(PrintBuf2[53]<<24);
	  
	   
	  //printf("\n��ǰʣ����Ϊ:%ld\n",Szt_deviceInf.Curr_RemainMoney);


	  for(i=0;i<28;i++)
	  {
	    Szt_deviceInf.Curr_ParFileName[i] = PrintBuf2[54 +i];
	  }
	  /*
	  printf("\n��ǰ��Ȳ�����Ϊ��");
	  for(i=0;i<28;i++)
	  {
	     printf("%c",Szt_deviceInf.Curr_ParFileName[i]);
	  }
	  */

	  
	  Szt_deviceInf.Curr_OKRecNum = PrintBuf2[82] + (PrintBuf2[83]<<8);
	  //printf("\n��ǰδ�ϴ��Ľ��׼�¼��Ϊ��%d\n",Szt_deviceInf.Curr_OKRecNum);


	 
	  Szt_deviceInf.Curr_BadRecNum = PrintBuf2[84] + (PrintBuf2[85]<<8);
	 
	  
	  //printf("\n��ǰδ�ϴ��Ľ��׼�¼��Ϊ��%d\n",Szt_deviceInf.Curr_BadRecNum );

	  for(i=0;i<8;i++)
	  {
	     Szt_deviceInf.Curr_DeviceTime[i] = PrintBuf2[86 +i];
	  }
	  //printf("\n��ǰ�豸ʱ��Ϊ��%x%x��%x��%x��%xʱ%x��%x��\n",Szt_deviceInf.Curr_DeviceTime[0],Szt_deviceInf.Curr_DeviceTime[1],Szt_deviceInf.Curr_DeviceTime[2],Szt_deviceInf.Curr_DeviceTime[3],Szt_deviceInf.Curr_DeviceTime[4],Szt_deviceInf.Curr_DeviceTime[5],Szt_deviceInf.Curr_DeviceTime[6]);
	  //���ڿɲ���ʾ�����ԣ���0x03��ʾ������

	  /*
	  if( Szt_deviceInf.Device_Status[0] & 0x80 )
	  {
	    printf("\n��ǰ��¼��\n");
	  }
	  else
	  {
	   printf("\n��ǰ��¼δ��\n"); 
	  }

	  if( Szt_deviceInf.Device_Status[0] & 0x40 )
	  {
	    printf("\n��ǰ��δ�ϴ���¼\n");
	  }
	  else
	  {
	    printf("\n��ǰû��δ�ϴ���¼\n");
	  }


	  if( Szt_deviceInf.Device_Status[0] & 0x20 )
	  {
	    printf("\n��������Ч����\n");
	  }
	  else
	  {
	    printf("\n��������Ч�򲻴���\n");
	  }

	  
	  if( Szt_deviceInf.Device_Status[0] & 0x10 )
	  {
	    printf("\nϵͳ�û�������Ч\n");
	  }
	  else
	  {
	     printf("\nϵͳ�û�������Ч�򲻴���\n");
	  }

	  if( Szt_deviceInf.Device_Status[1] & 0x08 )
	  {
	    printf("\n����ǩ��\n");
	  }
	  else
	  {
	    printf("\n����ǩ��\n");
	  }

	  if( Szt_deviceInf.Device_Status[1] & 0x04 )
	  {
	    printf("\n�豸ʱ����ϣ���Ҫͬ��ʱ��\n");
	  }
	  else
	  {
	    printf("\n���豸Ӳ������\n");
	  }

	  if( Szt_deviceInf.Device_Status[1] & 0x02 )
	  {
	    printf("\n�����Ȩ����\n");
	  }
	  else
	  {
	    printf("\n�����Ȩ���ڻ���Ч\n");
	  }


	  if( Szt_deviceInf.Device_Status[1] & 0x01 )
	  {
	    printf("\n�����ϴ�ϵͳӦ������\n");
	  }
	  else
	  {
	    printf("\n�������ϴ�ϵͳӦ������\n");
	  }
	  */

	   
   }
   else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\n״̬��ѯʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n״̬��ѯʧ��2\n");
   }
   else
   {
     //printf("\n״̬��ѯʧ��3\n");
   }

   return PrintBuf2[27]+1;
}



/*��������ͨ����Ա��¼,����1�ɹ�������0ʧ��*/
uint8_t ManageSztUserLogin(void)
{
  uint8_t temp =0;
  if(PrintBuf2[27] == 0x00)
	{
	  //printf("\n����Ա��¼�ɹ�");
	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
  {
	  printf("\n����Ա��¼ʧ��1\n");
  }
  else if( PrintBuf2[27] == 0x51)	//��������
  {
  	//printf("\n����Ա��¼ʧ��2\n");
  }
  else
  {
  	//printf("\n����Ա��¼ʧ��3\n");
  }

  temp = PrintBuf2[27] +1;

  return temp;

}







/*��������ͨʱ��ͬ��,û��ͬ�������Ϣ(2014.2.11),����1�ɹ�������0ʧ��*/
uint8_t ManageSztTimeCheck(void)
{
  uint8_t temp =0;
  TimeResult = 0xff;
  if(PrintBuf2[27] == 0x00)
  	{
  	  //printf("\nʱ��ͬ���ɹ�");
	  TimeResult = PrintBuf2[27]; //2014.2.11
	  /*
	  if( TimeResult == 0x00)
	  	{
	  	  printf("\nʱ��ͬ���ɹ�");
	  	}
	  else if( TimeResult == 0x01)
	  	{
	  	  printf("\n�����ʱ����Ϣ��ǰ̫��");
	  	}
	  else if( TimeResult == 0x02)
	  	{
	  	  printf("\n�����ʱ����Ϣ�ͺ�̫��");
	  	}
	  else if( TimeResult == 0x03)
	  	{
	  	  printf("\n�����ʱ����Ϣ��Ч");
	  	}
	  else if( TimeResult == 0x04)
	  	{
	  	  printf("\nʱ��δͬ��");
	  	}
	  else
	  	{
	  	  printf("\nʱ��ͬ����������:%d",TimeResult);
	  	}
	  	 */

	  MachineTime.Year_H = PrintBuf2[28];
	  MachineTime.Year_L = PrintBuf2[29];
	  MachineTime.Month = PrintBuf2[30];
	  MachineTime.Day = PrintBuf2[31];
	  MachineTime.Hours = PrintBuf2[32];
	  MachineTime.Minutes = PrintBuf2[33];
	  MachineTime.Senconds = PrintBuf2[34];
	  MachineTime.WeekDay = PrintBuf2[35];
	   /*
	  tmp_yy = MachineTime.Year_L;
	  tmp_m2 = MachineTime.Month;
	  tmp_dd = MachineTime.Day;
	  tmp_hh = MachineTime.Hours;
	  tmp_mm = MachineTime.Minutes;
	  tmp_ss = MachineTime.Senconds;
	  tmp_ww = MachineTime.WeekDay;
	  RTC_TimeRegulate();
*/
	 // printf("\n��ǰ�豸ʱ��Ϊ:%2x%2x-%x-%x %x:%x:%x",MachineTime.Year_H,MachineTime.Year_L,MachineTime.Month,MachineTime.Day,MachineTime.Hours,MachineTime.Minutes,MachineTime.Senconds);

	 //�¼��޸�ϵͳʱ��
	 tmp_yy = BCDtoDec(MachineTime.Year_L);
	 tmp_m2 = BCDtoDec(MachineTime.Month);
	 tmp_dd = BCDtoDec(MachineTime.Day);
	 tmp_hh = BCDtoDec(MachineTime.Hours);
	 tmp_mm = BCDtoDec(MachineTime.Minutes);
	 tmp_ss = BCDtoDec(MachineTime.Senconds);
	 tmp_ww = BCDtoDec(MachineTime.WeekDay);
	 //printf("\n   time:%d-%d-%d %d:%d:%d",tmp_yy,tmp_m2,tmp_dd,tmp_hh,tmp_mm,tmp_ss);
	 RTC_TimeRegulate();  
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\nʱ��ͬ��ʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\nʱ��ͬ��ʧ��2\n");
   }
   else
   {
     //printf("\nʱ��ͬ��ʧ��3\n");
   }

   temp = PrintBuf2[27] +1;

   return temp;

}



/*��������ͨ���οۿ�,����1�ɹ�������0ʧ��*/
uint8_t ManageSztDeductOnce(void)
{
  uint8_t temp =0;
  if(PrintBuf2[27] == 0x00)
  	{
  	   //printf("\n����ͨ�״οۿ�ɹ�\n");
	   SztReductInf.BeginMoney = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ�ۿ�ǰ���Ϊ%ld\n",SztReductInf.BeginMoney);
	   SztReductInf.EndMoney = PrintBuf2[32] + (PrintBuf2[33]<<8) + (PrintBuf2[34]<<16) + (PrintBuf2[35]<<24);
	   //printf("\n����ͨ�ۿ�����Ϊ%ld\n",SztReductInf.EndMoney);
	   SztReductInf.CardNum = PrintBuf2[36] + (PrintBuf2[37]<<8) + (PrintBuf2[38]<<16) + (PrintBuf2[39]<<24);
	   //printf("\n����ͨ����Ϊ%d\n",SztReductInf.CardNum);
	   SztReductInf.TradeNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν������Ϊ%d\n",SztReductInf.TradeNum);
	   SztReductInf.TACNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν���TAC��Ϊ%d\n",SztReductInf.TACNum);
	   SztReductInf.MachineAmount = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸��Ȩ���Ϊ%d\n",SztReductInf.MachineAmount);
	   SztReductInf.machineNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸�ն˱��Ϊ%d\n",SztReductInf.machineNum);
  	  
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\n����ͨ�ۿ�ʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n����ͨ�ۿ�ʧ��2\n");
   }
   else
   {
     //printf("\n����ͨ�ۿ�ʧ��3\n");
   }

   temp = PrintBuf2[27] +1;

   return temp;

}





/*��������ͨ�ٴοۿ�,����1�ɹ�������0ʧ��*/
uint8_t ManageSztDeductAgain(void)
{
  uint8_t temp =0;
  if(PrintBuf2[27] == 0x00)
  	{
  	   //printf("\n����ͨ�״οۿ�ɹ�\n");
	   SztReductInf.BeginMoney = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ�ۿ�ǰ���Ϊ%ld\n",SztReductInf.BeginMoney);
	   SztReductInf.EndMoney = PrintBuf2[32] + (PrintBuf2[33]<<8) + (PrintBuf2[34]<<16) + (PrintBuf2[35]<<24);
	   //printf("\n����ͨ�ۿ�����Ϊ%ld\n",SztReductInf.EndMoney);
	   SztReductInf.CardNum = PrintBuf2[36] + (PrintBuf2[37]<<8) + (PrintBuf2[38]<<16) + (PrintBuf2[39]<<24);
	   //printf("\n����ͨ����Ϊ%d\n",SztReductInf.CardNum);
	   SztReductInf.TradeNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν������Ϊ%d\n",SztReductInf.TradeNum);
	   SztReductInf.TACNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν���TAC��Ϊ%d\n",SztReductInf.TACNum);
	   SztReductInf.MachineAmount = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸��Ȩ���Ϊ%d\n",SztReductInf.MachineAmount);
	   SztReductInf.machineNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸�ն˱��Ϊ%d\n",SztReductInf.machineNum);
  	  
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\n����ͨ�ۿ�ʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n����ͨ�ۿ�ʧ��2\n");
   }
   else
   {
     //printf("\n����ͨ�ۿ�ʧ��3\n");
   }

   temp = PrintBuf2[27] +1;

   return temp;

}




/*��������ͨ�״ζ���,����1�ɹ�������0ʧ��*/
uint8_t ManageSztReadCardOnce(void)
{
  uint8_t temp =0;
  uint8_t i;
  if(PrintBuf2[27] == 0x00)
  {
	  //printf("\n����ͨ�״ζ����ɹ�\n");
	  SztCardInf.CardStatus = PrintBuf2[28];
	  //printf("\n��״̬Ϊ:%d",SztCardInf.CardStatus);
	  SztCardInf.CardNum = PrintBuf2[29] + (PrintBuf2[30]<<8) + (PrintBuf2[31]<<16) + (PrintBuf2[32]<<24);
	  //printf("\n����Ϊ:%d",SztCardInf.CardNum);
	  SztCardInf.CardType = PrintBuf2[33];
	  //printf("\n������Ϊ:%d",SztCardInf.CardStatus);
	  SztCardInf.SztMoney = PrintBuf2[34] + (PrintBuf2[35]<<8) + (PrintBuf2[36]<<16) + (PrintBuf2[37]<<24);
	  //printf("\n����ͨ��ǰ���Ϊ:%ld",SztCardInf.SztMoney);
	  SztCardInf.MachinMoney = PrintBuf2[38] + (PrintBuf2[39]<<8) + (PrintBuf2[40]<<16) + (PrintBuf2[41]<<24);
	  //printf("\n�豸��ǰʣ����Ϊ:%ld",SztCardInf.MachinMoney);
	  SztCardInf.SztDeposit = PrintBuf2[42] + (PrintBuf2[43]<<8) + (PrintBuf2[44]<<16) + (PrintBuf2[45]<<24);
	  //printf("\n����ͨѺ��Ϊ:%d",SztCardInf.SztDeposit);
	  for(i=0;i<4;i++)
	  {
		  SztCardInf.SellCardDay[i] = PrintBuf2[46+i];
	  }
	  //printf("\n����ͨ��������Ϊ:%x%x-%x-%x",SztCardInf.SellCardDay[0],SztCardInf.SellCardDay[1],SztCardInf.SellCardDay[2],SztCardInf.SellCardDay[3]);
	  SztCardInf.GetRentsMonth = PrintBuf2[50];
	  //printf("\n�������·�Ϊ:%d",SztCardInf.GetRentsMonth);
	  SztCardInf.RentsNum = PrintBuf2[51] + (PrintBuf2[52]<<8) + (PrintBuf2[53]<<16) + (PrintBuf2[54]<<24);
	  //printf("\nӦ�����Ϊ:%d",SztCardInf.RentsNum);
	  SztCardInf.ReturnableMoney = PrintBuf2[55] + (PrintBuf2[56]<<8) + (PrintBuf2[57]<<16) + (PrintBuf2[58]<<24);
	  //printf("\n������ʵ�ʽ��Ϊ:%ld",SztCardInf.ReturnableMoney);
	  SztCardInf.Poundage = PrintBuf2[59] + (PrintBuf2[60]<<8) + (PrintBuf2[61]<<16) + (PrintBuf2[62]<<24);
	  //printf("\n�˿�������Ϊ:%ld",SztCardInf.Poundage);
	  SztCardInf.NoSellCardDeposit = PrintBuf2[63] + (PrintBuf2[64]<<8) + (PrintBuf2[65]<<16) + (PrintBuf2[66]<<24);

	  //printf("\nδ�ۿ�Ѻ��Ϊ:%ld",SztCardInf.NoSellCardDeposit);
	  SztCardInf.NoSellCardMoney = PrintBuf2[67] + (PrintBuf2[68]<<8) + (PrintBuf2[69]<<16) + (PrintBuf2[70]<<24);
	  //printf("\nδ�ۿ����Ϊ:%ld",SztCardInf.NoSellCardMoney);
	  
  }
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
  {
     //printf("\n����ͨ�״ζ���ʧ��1\n");
  }
  else if( PrintBuf2[27] == 0x51)  //��������
  {
     //printf("\n����ͨ�״ζ���ʧ��2\n");
  }
  else
  {
     //printf("\n����ͨ�״ζ���ʧ��3\n");
  }

  temp = PrintBuf2[27] +1;

  return temp;

}





/*��������ͨ�ٴζ���,����1�ɹ�������0ʧ��*/
uint8_t ManageSztReadCardAgain(void)
{
  uint8_t temp =0;
  uint8_t i;
  if(PrintBuf2[27] == 0x00)
  	{
  	  //printf("\n����ͨ�ٴζ����ɹ�\n");
  	  SztCardInf.CardStatus = PrintBuf2[28];
  	  //printf("\n��״̬Ϊ:%d",SztCardInf.CardStatus);
  	  SztCardInf.CardNum = PrintBuf2[29] + (PrintBuf2[30]<<8) + (PrintBuf2[31]<<16) + (PrintBuf2[32]<<24);
  	  //printf("\n����Ϊ:%d",SztCardInf.CardNum);
  	  SztCardInf.CardType = PrintBuf2[33];
  	  //printf("\n������Ϊ:%d",SztCardInf.CardStatus);
  	  SztCardInf.SztMoney = PrintBuf2[34] + (PrintBuf2[35]<<8) + (PrintBuf2[36]<<16) + (PrintBuf2[37]<<24);
  	  //printf("\n����ͨ��ǰ���Ϊ:%ld",SztCardInf.SztMoney);
  	  SztCardInf.MachinMoney = PrintBuf2[38] + (PrintBuf2[39]<<8) + (PrintBuf2[40]<<16) + (PrintBuf2[41]<<24);
  	  //printf("\n�豸��ǰʣ����Ϊ:%ld",SztCardInf.MachinMoney);
  	  SztCardInf.SztDeposit = PrintBuf2[42] + (PrintBuf2[43]<<8) + (PrintBuf2[44]<<16) + (PrintBuf2[45]<<24);
  	  //printf("\n����ͨѺ��Ϊ:%d",SztCardInf.SztDeposit);
  	  for(i=0;i<4;i++)
  	  	{
  		  SztCardInf.SellCardDay[i] = PrintBuf2[46+i];
  	  	}
  	  //printf("\n����ͨ��������Ϊ:%x%x-%x-%x",SztCardInf.SellCardDay[0],SztCardInf.SellCardDay[1],SztCardInf.SellCardDay[2],SztCardInf.SellCardDay[3]);
  	  SztCardInf.GetRentsMonth = PrintBuf2[50];
  	  //printf("\n�������·�Ϊ:%d",SztCardInf.GetRentsMonth);
  	  SztCardInf.RentsNum = PrintBuf2[51] + (PrintBuf2[52]<<8) + (PrintBuf2[53]<<16) + (PrintBuf2[54]<<24);
  	  //printf("\nӦ�����Ϊ:%d",SztCardInf.RentsNum);
  	  SztCardInf.ReturnableMoney = PrintBuf2[55] + (PrintBuf2[56]<<8) + (PrintBuf2[57]<<16) + (PrintBuf2[58]<<24);
  	  //printf("\n������ʵ�ʽ��Ϊ:%ld",SztCardInf.ReturnableMoney);
  	  SztCardInf.Poundage = PrintBuf2[59] + (PrintBuf2[60]<<8) + (PrintBuf2[61]<<16) + (PrintBuf2[62]<<24);
  	  //printf("\n�˿�������Ϊ:%ld",SztCardInf.Poundage);
  	  SztCardInf.NoSellCardDeposit = PrintBuf2[63] + (PrintBuf2[64]<<8) + (PrintBuf2[65]<<16) + (PrintBuf2[66]<<24);
  
  	  //printf("\nδ�ۿ�Ѻ��Ϊ:%ld",SztCardInf.NoSellCardDeposit);
  	  SztCardInf.NoSellCardMoney = PrintBuf2[67] + (PrintBuf2[68]<<8) + (PrintBuf2[69]<<16) + (PrintBuf2[70]<<24);
  	  //printf("\nδ�ۿ����Ϊ:%ld",SztCardInf.NoSellCardMoney);
    
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
  {
     //printf("\n����ͨ�ٴζ���ʧ��1\n");
  }
  else if( PrintBuf2[27] == 0x51)  //��������
  {
     //printf("\n����ͨ�ٴζ���ʧ��2\n");
  }
  else
  {
     //printf("\n����ͨ�ٴζ���ʧ��3\n");
  }

  temp = PrintBuf2[27] +1;

  return temp;

}




/*��������ͨ�Ͽ�����,����1�ɹ�������0ʧ��*/
uint8_t ManageSztUnlink(void)
{
  uint8_t temp =0;
  
  if(PrintBuf2[27] == 0x00)
  	{
  	  //printf("\n�Ͽ����ӳɹ�\n");
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\n�Ͽ�����ʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n�Ͽ�����ʧ��2\n");
   }
   else
   {
     //printf("\n�Ͽ�����ʧ��3\n");
   }
  /* */

   temp = PrintBuf2[27] +1;

   return temp;

}




/*��������ͨ����ǩ�����������ء������ϴ�,����1�ɹ�������0ʧ��*/
uint8_t ManageSztAutoManage(void)
{
  uint8_t temp =0;
  
  if(PrintBuf2[27] == 0x00)
  	{
  	  //printf("\n����ǩ����������ػ������ϴ��ɹ�\n");
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\n����ǩ����������ػ������ϴ�ʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n����ǩ����������ػ������ϴ�ʧ��2\n");
   }
   else
   {
     //printf("\n����ǩ����������ػ������ϴ�ʧ��3\n");
   }
   /**/

   temp = PrintBuf2[27] +1;

   return temp;

}




/*��������ͨ��ֵ����,����1�ɹ�������0ʧ��*/
uint8_t ManageSztRechargeOnce(void)
{
  uint8_t temp =0;
  if(PrintBuf2[27] == 0x00)
  	{
  	   //printf("\n����ͨ�״γ�ֵ�����ɹ�\n");
	   SztReductInf.BeginMoney = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ�ۿ�ǰ���Ϊ%ld\n",SztReductInf.BeginMoney);
	   SztReductInf.EndMoney = PrintBuf2[32] + (PrintBuf2[33]<<8) + (PrintBuf2[34]<<16) + (PrintBuf2[35]<<24);
	   //printf("\n����ͨ�ۿ�����Ϊ%ld\n",SztReductInf.EndMoney);
	   SztReductInf.CardNum = PrintBuf2[36] + (PrintBuf2[37]<<8) + (PrintBuf2[38]<<16) + (PrintBuf2[39]<<24);
	   //printf("\n����ͨ����Ϊ%d\n",SztReductInf.CardNum);
	   SztReductInf.TradeNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν������Ϊ%d\n",SztReductInf.TradeNum);
	   SztReductInf.TACNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν���TAC��Ϊ%d\n",SztReductInf.TACNum);
	   SztReductInf.MachineAmount = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸��Ȩ���Ϊ%d\n",SztReductInf.MachineAmount);
	   SztReductInf.machineNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸�ն˱��Ϊ%d\n",SztReductInf.machineNum);
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\nʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n״̬��ѯʧ��2\n");
   }
   else
   {
     //printf("\n״̬��ѯʧ��3\n");
   }

   temp = PrintBuf2[27] +1;

   return temp;

}





/*��������ͨ��ֵ����,����1�ɹ�������0ʧ��*/
uint8_t ManageSztRechargeAgain(void)
{
  uint8_t temp =0;
  if(PrintBuf2[27] == 0x00)
  	{
  	   //printf("\n����ͨ�ٴγ�ֵ�����ɹ�\n");
	   SztReductInf.BeginMoney = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ�ۿ�ǰ���Ϊ%ld\n",SztReductInf.BeginMoney);
	   SztReductInf.EndMoney = PrintBuf2[32] + (PrintBuf2[33]<<8) + (PrintBuf2[34]<<16) + (PrintBuf2[35]<<24);
	   //printf("\n����ͨ�ۿ�����Ϊ%ld\n",SztReductInf.EndMoney);
	   SztReductInf.CardNum = PrintBuf2[36] + (PrintBuf2[37]<<8) + (PrintBuf2[38]<<16) + (PrintBuf2[39]<<24);
	   //printf("\n����ͨ����Ϊ%d\n",SztReductInf.CardNum);
	   SztReductInf.TradeNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν������Ϊ%d\n",SztReductInf.TradeNum);
	   SztReductInf.TACNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n����ͨ���ν���TAC��Ϊ%d\n",SztReductInf.TACNum);
	   SztReductInf.MachineAmount = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸��Ȩ���Ϊ%d\n",SztReductInf.MachineAmount);
	   SztReductInf.machineNum = PrintBuf2[28] + (PrintBuf2[29]<<8) + (PrintBuf2[30]<<16) + (PrintBuf2[31]<<24);
	   //printf("\n�豸�ն˱��Ϊ%d\n",SztReductInf.machineNum); 
  	}
  else if( PrintBuf2[27] == 0x88)	 //������Ҫ�жϷ����������ǰ���ֽڴ������
   {
     //printf("\nʧ��1\n");
   }
   else if( PrintBuf2[27] == 0x51)  //��������
   {
     //printf("\n״̬��ѯʧ��2\n");
   }
   else
   {
     //printf("\n״̬��ѯʧ��3\n");
   }

   temp = PrintBuf2[27] +1;

   return temp;

}




/*����������ʼ��ǩ��,����1�ɹ�������0ʧ��*/
uint8_t ManageGpbocInit(void)
{
  uint8_t i;
  uint8_t j;

  if( PrintBuf2[27] == 0x00)	 //����
   {
     //printf("\n����ǩ���ɹ�\n");
	 GpbocSamNum[0] = PrintBuf2[12];
	 GpbocSamNum[1] = PrintBuf2[13];
	 GpbocSamNum[2] = PrintBuf2[14];
	 GpbocSamNum[3] = PrintBuf2[15];

	 

	 UpInitErrNum[0] = PrintBuf2[28];	 
	 UpInitErrNum[1] = PrintBuf2[29];
	 
	 UpInitGpbocErrNum[0] = PrintBuf2[30];
	 UpInitGpbocErrNum[1] = PrintBuf2[31];

	 for(j=0;j<10;j++)
	 	{
		 	 UpInitData[j].ucLogInLogVersion = PrintBuf2[32 + j*34];
			 UpInitData[j].ucAcqBankNum = PrintBuf2[33 + j*34];
			 UpInitData[j].ucLogInOK = PrintBuf2[34 + j*34];
			 UpInitData[j].ucOper = PrintBuf2[35 + j*34];
			 UpInitData[j].ucKeyMode = PrintBuf2[36 + j*34];
			 
			 UpInitData[j].lBatchNumber = PrintBuf2[37 + j*34] + (PrintBuf2[38 + j*34]<<8) + (PrintBuf2[39 + j*34]<<16) + (PrintBuf2[40 + j*34]<<24);
			 
			 for(i=0;i<9;i++)
			 	{
			 	  UpInitData[j].szPosId[i] = PrintBuf2[41 +i + j*34] ;
			 	}

			 for(i=0;i<16;i++)
			 	{
			 	  UpInitData[j].szMerchantId[i] = PrintBuf2[50 +i + j*34];
			 	}
	 	}
	 /*
	 printf("\n�ṹ��汾��Ϊ:%d\n",UpInitData[0].ucLogInLogVersion);
	 printf("\n�յ��к�:%d\n",UpInitData[0].ucAcqBankNum);
	 printf("\nǩ���ɹ���־:%d\n",UpInitData[0].ucLogInOK);
	 printf("\n����Ա:%d\n",UpInitData[0].ucOper);
	 printf("\nǩ����Կ���ͣ�(0��ʾ˫������1��ʾ������):%c\n",UpInitData[0].ucKeyMode);
	 
	 printf("\n���κ�Ϊ:%d\n",UpInitData[0].lBatchNumber);
	 printf("\n\nPOS�ն˺�Ϊ:");
	 for(i=0;i<9;i++)
	 {
	   printf("%c",UpInitData[0].szPosId[i]);
	 }
	 delay_us(5);
	 printf("\n\n�̻���Ϊ:");
	 for(i=0;i<16;i++)
	 {
	   printf("%c",UpInitData[0].szMerchantId[i]);
	 }
	   */

   }
  
    else if( PrintBuf2[27] == 0x88)  //������Ҫ�жϷ����������ǰ���ֽڴ������
	 {
	   //printf("\n����ǩ��ʧ��1\n");
	 }
	 else if( PrintBuf2[27] == 0x51)  //��������
	 {
	  //printf("\n����ǩ��ʧ��2\n");
	 }
	 else
	 {
	   //printf("\n����ǩ��ʧ��3\n");
	 }


	 return PrintBuf2[27]+1;
  
}






/*������ѯ���������,����1�ɹ�������0ʧ��*/
uint8_t ManageGpbocReadCard(void)
{
  uint8_t i;
  uint8_t temp=0;
  
  
  if( PrintBuf2[27] == 0x00)	 //����
   {
     //printf("\n������ȡ���ɹ�\n");

	 //����Ϊ�ѽ��յ������浽��Ӧ��������
	 UpInitErrNum[0] = PrintBuf2[28];	 
	 UpInitErrNum[1] = PrintBuf2[29];
	 
	 UpInitGpbocErrNum[0] = PrintBuf2[30];
	 
	 UpInitGpbocErrNum[1] = PrintBuf2[31];

	 UpReadCardData.SztStatus = PrintBuf2[32];
	 
	 UpReadCardData.SztNum = PrintBuf2[33] + (PrintBuf2[34]<<8) + (PrintBuf2[35]<<16) + (PrintBuf2[36]<<24);
	 
	 UpReadCardData.SztMoney = PrintBuf2[37] + (PrintBuf2[38]<<8) + (PrintBuf2[39]<<16) + (PrintBuf2[40]<<24);
	 //printf("\ncard money is %ld",UpReadCardData.SztMoney);
	 
	 for(i=0;i<64;i++)
	 	{
	 	  UpReadCardData.SztReserve[i] = PrintBuf2[41+i];
	 	}
	 /*
	 if( UpReadCardData.SztStatus ==0 )
	 	{
	 	  printf("\n����������ͨ����\n");
	 	}
	 else if (UpReadCardData.SztStatus == 0x03)
	 	{
	 	  printf("\n����ͨΪ������\n");
	 	}
	 else if( UpReadCardData.SztStatus == 0x09)
	 	{
	 	  printf("\n����ͨΪ��������\n");
	 	}
	 else if( UpReadCardData.SztStatus == 0x07)
	 	{
	 	  printf("\n����ͨΪ���տ�\n");
	 	}
	 else 
	 	{
	 	  printf("\n����ͨ��Ч\n");
	 	}

	 if(UpReadCardData.SztStatus != 0x00)
	 	{
	 	  printf("\n����ͨ����Ϊ:%d\n",UpReadCardData.SztNum);
		  
		  printf("\n����ͨ���Ϊ:%ld\n",UpReadCardData.SztMoney);		
	 	}
	  */

	 

	 UpReadCardData.GpbocStatus = PrintBuf2[105];
	 if( UpReadCardData.GpbocStatus != 0)
	 {
		 for(i=0;i<20;i++)
		 	{
		 	  UpReadCardData.GpbocNum[i] = PrintBuf2[106+i];
		 	}
			/*
		 printf("\n��������Ϊ:");
		 for(i=0;i<20;i++)
			{
			  printf("%c",UpReadCardData.GpbocNum[i]);
			}
			  */
		 
		 UpReadCardData.GpbocMoney = PrintBuf2[126] +(PrintBuf2[127]<<8) + (PrintBuf2[128]<<16) + (PrintBuf2[129]<<24);
		 
		 
		 for(i=0;i<64;i++)
		 	{
		 	  UpReadCardData.GpbocReserve[i] = PrintBuf2[130+i];
		 	}
			delay_us(5);
		//printf("\n�����������Ϊ:%ld\n",UpReadCardData.GpbocMoney);
		 /*
		 if( UpReadCardData.GpbocStatus != 0)
		 	{
		 	  printf("\n��������Ϊ:");
			  for(i=0;i<20;i++)
			  	{
			  	  printf("%c",UpReadCardData.GpbocNum[i]);
			  	}
		
			  delay_us(5);
		
			  printf("\n�����������Ϊ:%ld\n",UpReadCardData.GpbocMoney);
		 	}
		 else
		 	{
		 	  printf("\n��������������\n");
		 	}
		   */
		 }
		 else 
		 {
		   //printf("\n��������������");
		 }
   }
  
  else if( PrintBuf2[27] == 0x88)  //������Ҫ�жϷ����������ǰ���ֽڴ������
	 {
	   //printf("\n����������ѯ���ʧ��1\n");
	 }
  else if( PrintBuf2[27] == 0x51)  //��������
	 {
	   //printf("\n����������ѯ���ʧ��1\n");
	 }
  else
	 {
	   //printf("\n����������ѯ���ʧ��1\n");
	 }
  temp = PrintBuf2[27] +1;

  //printf("printBuf2[27]= %x",temp);
  
  
  return temp;
}





//uint32_t BackupsIndex;
/*�����ۿ������,����1�ɹ�������0ʧ��*/
uint8_t ManageGpbocDeduct(void)
{
  uint8_t i;
  uint32_t temp=0;
  if( PrintBuf2[27] == 0x00)	 //����
   {
     //printf("\n�����ۿ�ɹ�");

  	 //����Ϊ�ѽ��յ������浽��Ӧ��������
  	 UpInitErrNum[0] = PrintBuf2[28];	 
  	 UpInitErrNum[1] = PrintBuf2[29];
  	 
  	 UpInitGpbocErrNum[0] = PrintBuf2[30];
  	 UpInitGpbocErrNum[1] = PrintBuf2[31];
  
  
  	 UpDeductData.ucLogVersion = PrintBuf2[32];
  	 //printf("\n�����ۿ�ṹ��汾��Ϊ:%d\n",UpDeductData.ucLogVersion);
  	 
  	 UpDeductData.ucAcqBankNum = PrintBuf2[33];
  	 //printf("\n�����ۿ��յ��к�Ϊ:%d\n",UpDeductData.ucAcqBankNum);
  	 
  	 UpDeductData.iTransNo = PrintBuf2[34] + (PrintBuf2[35]<<8) + (PrintBuf2[36]<<16) + (PrintBuf2[37]<<24);
  	 //printf("\n�����ۿ�׺�Ϊ:%ld\n",UpDeductData.iTransNo);
  	  
  	 UpDeductData.ucBlklistFail = PrintBuf2[38];
  	 //printf("\n�����ۿ����������־Ϊ:%d\n",UpDeductData.ucBlklistFail);
  	 
  	 UpDeductData.ucqPBOCSaleFail = PrintBuf2[39];
  	 //printf("\n�����ۿ��ʧ�ܱ�־Ϊ:%d\n",UpDeductData.ucqPBOCSaleFail);
  	 
  	 UpDeductData.ucqPBOCLastRd = PrintBuf2[40];
  	 //printf("\n�����ۿ�����һ����¼ʧ�ܱ�־Ϊ:%d\n",UpDeductData.ucqPBOCLastRd);
  	 
  	 UpDeductData.ucOfflineSaleFail = PrintBuf2[41];
  	 //printf("\n�����ۿ��ѻ����׾ܾ���־Ϊ:%d\n",UpDeductData.ucOfflineSaleFail);
  	 
  	 UpDeductData.lTransAmount = PrintBuf2[42] + (PrintBuf2[43]<<8) + (PrintBuf2[44]<<16) + (PrintBuf2[45]<<24);
  	 //printf("\n�������ν��׽��Ϊ:%ld\n",UpDeductData.lTransAmount);
  	 
  	 UpDeductData.lCardBalance = PrintBuf2[46] + (PrintBuf2[47]<<8) + (PrintBuf2[48]<<16) + (PrintBuf2[49]<<24);
  	 //printf("\n��������EC���Ϊ:%ld\n",UpDeductData.lCardBalance);
  	 
  	 UpDeductData.lTraceNo = PrintBuf2[50] + (PrintBuf2[51]<<8) + (PrintBuf2[52]<<16) + (PrintBuf2[53]<<24);
  	 //printf("\n�����ۿ���ˮ��Ϊ:%ld\n",UpDeductData.lTraceNo);
  	 
  	 UpDeductData.lBatchNumber = PrintBuf2[54] + (PrintBuf2[55]<<8) + (PrintBuf2[56]<<16) + (PrintBuf2[57]<<24);
  	 //printf("\n�����ۿ����κ�Ϊ:%ld\n",UpDeductData.lBatchNumber);
  	 
  	 for(i=0;i<9;i++)
  	 	{
  	 	  UpDeductData.szPosId[i] = PrintBuf2[58+i];
  	 	}
  		/*
  	 printf("\nPOS�ն˺�Ϊ:");
  	 for(i=0;i<9;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szPosId[i]);
  	 	}
  		  */
  	 
  	 for(i=0;i<21;i++)
  	 	{
  	 	  UpDeductData.szCardNo[i] = PrintBuf2[67+i];
  	 	}
  		/*
  	 printf("\n����Ϊ:");
  	 for(i=0;i<21;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szCardNo[i]);
  	 	}
  		  */
  	 
  	 for(i=0;i<9;i++)
  	 	{
  	 	  UpDeductData.szDate[i] = PrintBuf2[88+i];
  	 	}
  		/*
  	 printf("\n\n�����ۿ�����Ϊ:");
  	 for(i=0;i<9;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szDate[i]);
  	 	}
  		  */
  	 
  	 for(i=0;i<7;i++)
  	 	{
  	 	  UpDeductData.szTime[i] = PrintBuf2[97+i];
  	 	}
  		/*
  	 printf("\n�����ۿ�ʱ��Ϊ:");	 
  	 for(i=0;i<7;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szTime[i]);
  	 	}
  		  */
  	 
  	 for(i=0;i<5;i++)
  	 	{
  	 	  UpDeductData.szExpDate[i] = PrintBuf2[104+i];
  	 	}
  		/*
  	 printf("\n\n�����ۿ�EXp����Ϊ:");	 
  	 for(i=0;i<5;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szExpDate[i]);
  	 	}
  		  */
  
  	 
  	 for(i=0;i<4;i++)
  	 	{
  	 	  UpDeductData.szTellerNo[i] = PrintBuf2[109+i];
  	 	}
  		/*
  	 printf("\n�����ۿ����ԱΪ:");
  	 
  	 for(i=0;i<4;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szTellerNo[i]);
  	 	}
  		  */
  
  	 
  	 for(i=0;i<4;i++)
  	 	{
  	 	  UpDeductData.szCardUnit[i] = PrintBuf2[113+i];
  	 	}
  		/*
  	 printf("\n������������֯Ϊ:");
  	 
  	 for(i=0;i<4;i++)
  	 	{
  	 	  printf("%c",UpDeductData.szCardUnit[i]);
  	 	}
  		  */
  	 
  	 
  	 UpDeductData.BkDataLen = PrintBuf2[117] + (PrintBuf2[118]<<8) + (PrintBuf2[119]<<16) + (PrintBuf2[120]<<24);
  	 //UpDeductData.BkDataLen = PrintBuf2[104] + (PrintBuf2[105]<<8);
  	  //printf("\n���ݽ������ݵĳ���Ϊ:%ld",UpDeductData.BkDataLen);
  
  	 // �������ݱ���
  	 
  	 for(temp =0;temp<UpDeductData.BkDataLen;temp++)
  	 	{
  	 	 // UpDedectBackup[temp] = PrintBuf2[121 + temp];  //unit32
  		  // UpDedectBackup[temp] = PrintBuf2[121 + temp];   //uint16
  	 	}
  	   
  	   delay_us(100);
  	  //printf("\n���ݽ����������");
	 
   }
  else if( PrintBuf2[27] == 0x88)  //������Ҫ�жϷ����������ǰ���ֽڴ������
	 {
	   //printf("\n�����ۿ�ʧ��1\n");
	   //return 0x88;
	 }
  else if( PrintBuf2[27] == 0x51)  //��������
	 {
	    //printf("\n�����ۿ�ʧ��2\n");
		//return 0x51;
	 }
  else
	 {
	   // printf("\n�����ۿ�ʧ��3\n");  

		
	 }

   //printf("printBuf2[27]= %x",PrintBuf2[27]);
  return PrintBuf2[27]+1;
  
	//return 0x00;
}




/*���������ϴ�����,����1�ɹ�������0ʧ��*/
uint8_t ManageGpbocSendData(void)
{
  uint8_t i;
  uint8_t temp=0xff ;
  if( PrintBuf2[27] == 0x00)	 //����
   {
     //printf("\n�����ϴ����ݳɹ�\n");

	 UpInitErrNum[0] = PrintBuf2[28];	 
	 UpInitErrNum[1] = PrintBuf2[29];
	 
	 UpInitGpbocErrNum[0] = PrintBuf2[30];
	 UpInitGpbocErrNum[1] = PrintBuf2[31];

	 UpSendData.ucUpLogVersion = PrintBuf2[32];
	  //printf("\n�����ϴ����ݵĽṹ��汾��Ϊ:%d\n",UpSendData.ucUpLogVersion);
	  
	 UpSendData.ucAcqBankNum = PrintBuf2[33];
	 //printf("\n�����ϴ����ݵ��յ��к�Ϊ:%d\n",UpSendData.ucAcqBankNum);
	 
	 UpSendData.ucUploadOK = PrintBuf2[34];
	 //printf("\n�����ϴ����ݵ�ǩ���ɹ���־Ϊ:%d\n",UpSendData.ucUploadOK);
	 
	 UpSendData.lBatchNumber = PrintBuf2[35]+ (PrintBuf2[36]<<8) + (PrintBuf2[37]<<16) + (PrintBuf2[38]<<24);
	 //printf("\n�����ϴ����ݵ����κ�Ϊ:%d\n",UpSendData.lBatchNumber);

	 
	 UpSendData.lTotalNum = PrintBuf2[39] + (PrintBuf2[40]<<8) + (PrintBuf2[41]<<16) + (PrintBuf2[42]<<24);
	 //printf("\n��Ҫ�����ļ��е��ܱ���Ϊ:%d\n",UpSendData.lTotalNum);

	 
	 UpSendData.lToUploadNum = PrintBuf2[43] + (PrintBuf2[44]<<8) + (PrintBuf2[45]<<16) + (PrintBuf2[46]<<24);
	 //printf("\n������Ҫ���͵��ܱ���:%d\n",UpSendData.lToUploadNum);

	 
	 UpSendData.lSuccNum = PrintBuf2[47] + (PrintBuf2[48]<<8) + (PrintBuf2[49]<<16) + (PrintBuf2[50]<<24);
	 //printf("\n�ɹ����͵ı���:%d\n",UpSendData.lSuccNum);

	 
	 UpSendData.lSuccTotalAmt = PrintBuf2[51] + (PrintBuf2[52]<<8) + (PrintBuf2[53]<<16) + (PrintBuf2[54]<<24);
	 //printf("\n�ɹ����͵��ܽ��:%d",UpSendData.lSuccTotalAmt);

	 
	 UpSendData.lFailNum = PrintBuf2[55] + (PrintBuf2[56]<<8) + (PrintBuf2[57]<<16) + (PrintBuf2[58]<<24);
	 //printf("\n����ʧ�ܵ��ܱ���:%d",UpSendData.lFailNum);

	 
	 UpSendData.lFailTotalAmt = PrintBuf2[59] + (PrintBuf2[60]<<8) + (PrintBuf2[61]<<16) + (PrintBuf2[62]<<24);
	 //printf("\n����ʧ�ܵ��ܽ��:%d",UpSendData.lFailTotalAmt);

	 
	 for(i=0;i<9;i++)
	 	{
	 	   UpSendData.szPosId[i] = PrintBuf2[63+i];
	 	}
		/*
	 printf("\nPOS�ն˺�Ϊ:");
	 for(i=0;i<9;i++)
	 	{
	 	  printf("%c",UpSendData.szPosId[i]);
	 	}
		  */
	 

	 for(i=0;i<16;i++)
	 	{
	 	   UpSendData.szMerchantId[i] = PrintBuf2[72+i];
	 	}
		/*
	 printf("\n\n�̻���Ϊ:");
	 for(i=0;i<16;i++)
	 	{
	 	  printf("%c",UpSendData.szMerchantId[i]);
	 	}
		  */
		 
   }
  else if( PrintBuf2[27] == 0x88)  //������Ҫ�жϷ����������ǰ���ֽڴ������
	 {
	   //printf("\n�����ϴ���¼ʧ��1\n");
	 }
  else if( PrintBuf2[27] == 0x51)  //��������
	 {
	   //printf("\n�����ϴ���¼ʧ��2\n");
	 }
  else
	 {
	   //printf("\n�����ϴ���¼ʧ��3\n");
	 }

  temp = PrintBuf2[27]+1; 
  return temp;

}







/*�����������ᡢǩ��*/
uint8_t ManageGpbocCheckOut(void)
{
  uint8_t i;
  if( PrintBuf2[27] == 0x00)	 //����
   {
     //printf("\n��������ǩ�˳ɹ�\n");

  	 UpInitErrNum[0] = PrintBuf2[28];	 
  	 UpInitErrNum[1] = PrintBuf2[29];
  	 
  	 UpInitGpbocErrNum[0] = PrintBuf2[30];
  	 UpInitGpbocErrNum[1] = PrintBuf2[31];
  
  	 UpCheckOutData.ucLogInLogVersion = PrintBuf2[32];
  	 //printf("\n��������ǩ�˵Ľṹ��汾��Ϊ:%d\n",UpCheckOutData.ucLogInLogVersion);
  	 
  	 UpCheckOutData.ucAcqBankNum = PrintBuf2[33];
  	 //printf("\n��������ǩ�˵��յ��к�Ϊ:%d\n",UpCheckOutData.ucAcqBankNum);
  	 
  	 UpCheckOutData.ucLogInOK = PrintBuf2[34];
  	  //printf("\n��������ǩ�˵�ǩ���ɹ���־Ϊ:%d\n",UpCheckOutData.ucLogInOK);
  	  
  	 UpCheckOutData.ucOper = PrintBuf2[35];
  	  //printf("\n��������ǩ�˵Ĳ���Ա��Ϊ:%d\n",UpCheckOutData.ucOper);
  	  
  	 UpCheckOutData.ucKeyMode = PrintBuf2[36];
  	 //printf("\n��������ǩ�����õ���Կ����Ϊ:%c\n",UpCheckOutData.ucKeyMode);
  	 
  	 UpCheckOutData.lBatchNumber = PrintBuf2[37] + (PrintBuf2[38]<<8) + (PrintBuf2[39]<<16) + (PrintBuf2[40]<<24);
  	 //printf("\n��������ǩ�˵����κ�Ϊ:%d\n",UpCheckOutData.lBatchNumber);
  	 
  	 for(i=0;i<9;i++)
  	 {
  		UpCheckOutData.szPosId[i] = PrintBuf2[41+i];
  	 }
  	 /*
  	 printf("\nPOS�ն˺�Ϊ:");
  	 for(i=0;i<9;i++)
  	 	{
  	 	  printf("%c",UpCheckOutData.szPosId[i]);
  	 	}
  	   */
  	 
  
     for(i=0;i<16;i++)
  	 {
  		UpCheckOutData.szMerchantId[i] = PrintBuf2[50+i];
  	 }
  	 /*
  	 printf("\n\n�̻���Ϊ:");
  	 for(i=0;i<16;i++)
  	 	{
  	 	  printf("%c",UpCheckOutData.szMerchantId[i]);
  	 	}
  	   */

   }
  else if( PrintBuf2[27] == 0x88)  //������Ҫ�жϷ����������ǰ���ֽڴ������
	 {
	   //printf("\n�������ᡢǩ��ʧ��1\n");
	 }
  else if( PrintBuf2[27] == 0x51)  //��������
	 {
	    //printf("\n�������ᡢǩ��ʧ��2\n");
	 }
  else
	 {
	    //printf("\n�������ᡢǩ��ʧ��3\n");
	 }

  return PrintBuf2[27]+1;

}




/*����*/
void ManageGpbocAll(uint8_t PutInNum)
{
  uint8_t orderFlag;
  
  if( PutInNum ==1 )
  {
    printf("PutInNum =1");
    Order_Ifn_QueryStatus();
  }
  else if( PutInNum == 2)
  {
    printf("PutInNum =2");
    Order_Gpboc_Init();
  }
  else if( PutInNum == 3)
  {
    printf("PutInNum =3");
    Order_Gpboc_ReadCard();
  }
   else if( PutInNum == 4)
  {
    printf("PutInNum =4");
    Order_Gpboc_Deduct(1);
  }
    else if( PutInNum == 5)
  {
    printf("PutInNum =5");
    Order_Gpboc_SendData();
  }
	 else if( PutInNum == 6)
  {
    printf("PutInNum =6");
    Order_Gpboc_Check_Out();
  }
  else
  {
    //printf("\nOver");
  }
  
  
  orderFlag = ManageReOrderType();
  if( orderFlag == 0)
  	{
  	  //printf("\n���󣬴����־Ϊ:1\n");
  	}
  else if( orderFlag == R_QueryStatus)
  	{
  	  ManageQueryStatus();
  	}
  else if( orderFlag == R_GpbocCheck_In)
  	{
  	  ManageGpbocInit();
  	}
  else if( orderFlag == R_GpbocReadCard)
  	{
  	  ManageGpbocReadCard();
  	}
  else if( orderFlag == R_GpbocDeduct)
  	{
  	  ManageGpbocDeduct();
  	}
  else if( orderFlag == R_GpbocSendData)
  	{
  	  ManageGpbocSendData();
  	}
  else if( orderFlag == R_GpbocCheck_Out)
  	{
  	  ManageGpbocCheckOut();
  	}
  else
  {
    //printf("\n���󣬴����־Ϊ:%d\n",orderFlag);
  }
  //PrintIndex2 = 0;
  //PrintBuf2[0] = 0xff;

  //Close_Szt_Gpboc();

  
}




/*����ǩ��,�ɹ�����1��ʧ�ܷ���0*/
uint8_t Gpboc_CheckIn (void)
{
  uint8_t temp;
  
  uint8_t flag1 =0;
  uint8_t flag2 =0;
  uint8_t flag3 =0;
  uint8_t flag4 =0;
  
  uint8_t result_End = 0;
  
  Order_Ifn_QueryStatus();
  temp = 0xff;
  temp = ManageReOrderType();
  if( temp == R_QueryStatus)
	{
	  flag1 = ManageQueryStatus();
	}
  delay_us(50);

  if( flag1 == 0x01)
  	{
  	  Order_Gpboc_Init();
  	  temp = 0xff;
  	  temp = ManageReOrderType();
  	  if( temp == R_GpbocCheck_In)
	  	{
	  	  flag2 = ManageGpbocInit();
	  	}
  
  		//printf("\n������ʼ����־=%x",flag2);
  		delay_ms(1000);
  
  	  if(flag2 == 0x01)
	  	{
	  	  Order_Gpboc_SendData();
  		  delay_ms(1000);
  		  temp = 0xff;
  		  temp = ManageReOrderType();
  		  if( temp == R_GpbocSendData)
		  	{
		  	  flag3 = ManageGpbocSendData();
			  
		  	}
		  //printf("\n�����ϴ���־=%x",flag3);
  		  delay_ms(1000);
  		  if( flag3 == 0x01)
		  	{
		  	  Order_Gpboc_Check_Out();
  			  delay_ms(1000);
  			  temp = 0xff;
  			  temp = ManageReOrderType();
  			  if( temp == R_GpbocCheck_Out)
			  	{
			  	  flag4 = ManageGpbocCheckOut();
			  	}
		  	}
		   	//printf("\n����ǩ�˱�־=%x",flag4);
  		}

  	  delay_ms(1000);
  	  if( flag4 == 0x01)
	  	{
	  	  Order_Gpboc_Init();
  		  temp = 0xff;
  		  temp = ManageReOrderType();
  		  if( temp == R_GpbocCheck_In)
		  	{
		  	  result_End = ManageGpbocInit();
		  	}
	  	}
  	}

	return result_End;
  
  
}




/*����ֻ�鿨������, �ɹ��鿨����1��ʧ�ܷ���0*/
uint8_t OnlyReadGpbocCard(void)
{
  uint8_t orderFlag1;
  uint8_t result1 =0;
  uint8_t endreturn =0;
  UpReadCardData.SztMoney =0;
  UpReadCardData.GpbocMoney =0;

  Order_Gpboc_ReadCard();
  orderFlag1 = ManageReOrderType();
  if( orderFlag1 == R_GpbocReadCard)
  	{
  	  result1 = ManageGpbocReadCard();
  	}

  if( result1 == 1)
  {
    endreturn =1;
  }

  return endreturn;
  
}



/*����������,����0��ʾ�ۿ�ʧ�ܣ�����1��ʾ�ۿ���ȷ*/
uint8_t GpbocDeduct(uint32_t money_deduct)
{
  uint8_t orderFlag1 =0;
  uint8_t result1 =0;
  uint8_t result2 =0;
  int32_t FristMoney=0;
	uint8_t cnt_t=0,i=0;
//   int32_t EndMoney=0;
//   int32_t FactMoney;
  //TRANSLOG_TO_HOST saveDeductIfn;

  //�����鿨
  UpDeductData.lTransAmount = 0;	  //��ۿ��ʼΪ0

	VariableChagelong(amountof_consumption,0);
	VariableChagelong(cardbalence_before,0);
	VariableChagelong(cardbalence_after,0);
	
loop5:	
	Order_Gpboc_ReadCard();
  orderFlag1 = ManageReOrderType();
  if( orderFlag1 == R_GpbocReadCard)
  {
  	result1 = ManageGpbocReadCard();
  }
  if( result1 == 0x01)          //�鿨�ɹ�
  {
  	FristMoney = UpReadCardData.GpbocMoney;
// 	    EndMoney = FristMoney;
       //����ʾ������ʾˢ��ǰ���UpReadCardData.GpbocMoney
    VariableChagelong(cardbalence_before,UpReadCardData.GpbocMoney);	
    if( FristMoney < money_deduct )
    {
      //printf("\n�������Ϊ%ld",FristMoney);
      //printf("�������㣬���ȳ�ֵ\r\n");
      return 0;
    }
    else
    {
      orderFlag1 =0;
	  	Order_Gpboc_Deduct(money_deduct);
			for(i=0;i<20;i++)
			{
				orderFlag1 = ManageReOrderType();
				if(orderFlag1 == R_GpbocDeduct)
				{
					result2 = ManageGpbocDeduct();
					if( result2 == 1)   //����ɹ�
					{
						//printf("����ɹ�\r\n");
						//����ʾ������ʾ�ۿ�������UpDeductData.lTransAmount���ۿ��UpDeductData.lCardBalance����
						VariableChagelong(amountof_consumption,UpDeductData.lTransAmount);
						VariableChagelong(cardbalence_after,UpDeductData.lCardBalance);	
						delay_ms(1000);
						return 1;
					}
					else
					{
						//printf("����ʧ��\r\n");
						return 0;
					}
				}
      }  
    }
  }
  else
  {
	  cnt_t++;
		DealSeriAceptData();
		if(cardbalence_cancel_flag)
		{
			cardbalence_cancel_flag=false;
			return 0;
		}
	  if(cnt_t<15)
			goto loop5;
    return 0;
  }
 }

/*�����������ݣ��ɹ����ͷ���1��ʧ�ܷ���0*/
uint8_t UpperGpboc(void)
{
  uint8_t orderFlag;
  uint8_t result1;
  uint8_t endresult =0;
  Order_Gpboc_SendData();
  orderFlag = ManageReOrderType();
  if( orderFlag == R_GpbocSendData)
  	{
  	  result1 = ManageGpbocSendData();
  	}
  else
  	{
  	  endresult= 0;  	  
  	}

  if(result1 == 1)
  	{
  	  //printf("\n�����������ݳɹ�\n");
  	  endresult =1;
  	}
  else
  	{
  	  //printf("\n������������ʧ��\n");
  	  endresult =0;
  	}
  return endresult;
}
/*�����������̣��ɹ����᷵��1��ʧ�ܷ���0*/
uint8_t GpbocVout(void)
{
  uint8_t orderFlag;
  uint8_t result1;
  uint8_t endresult =0;
  Order_Gpboc_Check_Out();
  orderFlag = ManageReOrderType();
  if( orderFlag == R_GpbocCheck_Out)
  	{
  	  result1 = ManageGpbocCheckOut();
  	}
  else
  	{
  	  endresult= 0;  	  
  	}

  if(result1 == 1)
  	{
  	  //printf("\n�������ᡢǩ�˳ɹ�\n");
  	  endresult =1;
  	}
  else
  	{
  	  //printf("\n�������ᡢǩ��ʧ��\n");
  	  
  	  endresult =0;
  	}

  return endresult;
}


void TestSzt(uint8_t PutInNum)
{
  uint8_t orderFlag;

  if( PutInNum ==1 )
  {
    //printf("PutInNum =1");
    Order_Ifn_QueryStatus();
  }
  else if( PutInNum == 2)
  {
    //printf("PutInNum =2");
    Order_SztUserLogin(); //��¼
  }
  else if( PutInNum == 3)
  {
    //printf("PutInNum =3");
    Order_SztTimeCheck(); //ʱ��ͬ��
  }
   else if( PutInNum == 4)
  {
    //printf("PutInNum =4");
    Order_SztAutoCheckIn();  //����ǩ��
  }
    else if( PutInNum == 5)
  {
    //printf("PutInNum =5");
    Order_SztAutoDownload();  //�������ز���
  }
	else if( PutInNum == 6)
  {
    //printf("PutInNum =6");
    Order_SztAutoSendData();  //�����ϴ�����
  }
	else if( PutInNum == 7)
  {
    //printf("PutInNum =7");
    Order_SztUnlink();  //�����ϴ�����
  }
	else if( PutInNum == 8)
  {
    //printf("PutInNum =8");
    //Order_SztReadCardOnce();  //�״ζ���
    Order_Gpboc_ReadCard();
  }
	else if( PutInNum == 9)
  {
    //printf("PutInNum =8");    
    Order_SztDeductOnce(1); //���οۿ�
  }
	else if( PutInNum == 10)
  {
    //printf("PutInNum =8");    
    Order_SztDeductAgain(1); //���οۿ�
  }
  else
  {
    //printf("\nOver");
  }
  
  
  orderFlag = ManageReOrderType();
  if( orderFlag == 0)
  	{
  	  //printf("\n���󣬴����־Ϊ:1\n");
  	}
  else if( orderFlag == R_QueryStatus)
  	{
  	  ManageQueryStatus();
  	}
  else if( orderFlag == R_UserLogin)
  	{
  	  ManageSztUserLogin();
  	}
  else if( orderFlag == R_TimeCheck)
  	{
  	  ManageSztTimeCheck();
  	}
  else if( orderFlag == R_SztCheck_In)
  	{
  	  ManageSztAutoManage();
  	}
  else if( orderFlag == R_SztUnlink)
  	{
  	  ManageSztUnlink();
  	}
  else if( orderFlag == R_GpbocReadCard)
  	{
  	  ManageGpbocReadCard();
  	}  
  else if( orderFlag == R_SztDeductOnce)
  	{
  	  ManageSztDeductOnce();
  	} 
  else if( orderFlag == R_SztDeductAgain)
  	{
  	  ManageSztDeductAgain();
  	} 
  else
  {
    //printf("\n���󣬴����־Ϊ:%d\n",orderFlag);
  }
}



/*����ͨǩ�����ɹ�����1��ʧ�ܷ���0*/
uint8_t Szt_CheckIn(void)
{
  uint8_t flag1 =0;//״̬��ѯ
  uint8_t flag2 =0;//ʱ��У��
  uint8_t flag3 =0;//����Ա��¼
  uint8_t flag4 =0;//����ͨ����ǩ��
  uint8_t flag5 =0;//����ͨ�������ز���
  uint8_t flag6 =0;//����ͨ�����ϴ�����
  uint8_t flag7 =0;//��ǩ���ɹ�����־
  uint8_t temp =0xff;

  Order_Ifn_QueryStatus();
  delay_us(10);
  temp = ManageReOrderType();
  if( temp == R_QueryStatus)       //״̬��ѯ
  	{
  	  flag1 = ManageQueryStatus();	  
  	}

  if(flag1 == 1)
  	{
	/**/
    	Order_SztTimeCheck();        //ʱ��У��
  	  delay_us(5);
  	  temp = 0xff;
  	  temp = ManageReOrderType();
  	  if( temp == R_TimeCheck )
  	  	{
  	  	  flag2 = ManageSztTimeCheck();
  	  	}
  	  
  	  Order_SztUserLogin();          //����Ա��¼
  	  delay_us(5);
  	  temp = 0xff;
  	  temp = ManageReOrderType();
  	  if( temp == R_UserLogin )
  	  	{
  	  	  flag3 = ManageSztUserLogin();
  	  	}

  	}
  
  if((flag1 == 1) && ( flag2 == 1) && ( flag3 ==1))
  	{
  	  Order_SztAutoCheckIn();              //����ͨ�Զ�ǩ��
  	  temp = 0xff;
  	  temp = ManageReOrderType();
  	  if( temp == R_SztCheck_In)
  	  	{
  	  	  flag4 = ManageSztAutoManage();			  
  	  	}
  
  	  if( flag4 == 1)
  	  	{
  		  Order_SztAutoDownload();            //����ͨ�Զ���������
  		  temp = 0xff;
  		  temp = ManageReOrderType();
  		  if( temp == R_SztCheck_In)
  		    {
  			  flag5 = ManageSztAutoManage();				
  		    }
  
  
  		  Order_SztAutoSendData();            //����ͨ�Զ��ϴ�����
  		  temp = 0xff;
  		  temp = ManageReOrderType();
  		  if( temp == R_SztCheck_In)
  		    {
  			  flag6 = ManageSztAutoManage();				
  		    }
  	  	}
  	}

  if( (flag1 ==1) && (flag2 ==1) && (flag3 ==1) && (flag4 ==1) && (flag5 ==1) && (flag6 ==1))
  	{
  	  flag7 = 1;  //ȫ�����ݶ�������ȷ��ǩ�����̲ųɹ�
  	}

  return flag7;
}


 

/*����ͨ�ۿ�*/
uint8_t SztDeduct(int32_t money)
{
  uint8_t temp ;
  uint8_t flag0 =0;
  uint8_t flag1 =0;
  uint8_t flag2 =0;
  uint8_t flag3 =0;
  uint8_t flag4 = 0;
  uint8_t endflag =0;
  uint8_t FristMoney =0;
  uint8_t EndMoney =0;
  int32_t TrueMoney=0;
	uint8_t cnt_t=0;
	uint8_t tempcount=0;

  SztReductInf.BeginMoney =0;
  SztReductInf.EndMoney =0;

	VariableChagelong(amountof_consumption,0);
	VariableChagelong(cardbalence_before,0);
	VariableChagelong(cardbalence_after,0);
	
	loop6:
  //�����鿨
  Order_Gpboc_ReadCard();
  temp = 0xff;
  temp = ManageReOrderType();
  if(temp == R_GpbocReadCard)
  {
  	flag0 = ManageGpbocReadCard();
  }
  if(flag0 == 0x01) //�ɹ�
  {
    FristMoney = UpReadCardData.SztMoney;			
    //��ʾ����ͨ��� UpReadCardData.SztMoney
	  VariableChagelong(cardbalence_before,UpReadCardData.SztMoney);
	  EndMoney = FristMoney;
  	Order_SztDeductOnce(money); //�ۿ�
  	delay_us(15);
  	temp = 0xff;
  	temp = ManageReOrderType(); 
  	if(temp == R_SztDeductOnce)
  	{
  	  flag1 = ManageSztDeductOnce();
  		if( flag1 == 1)
  		{
  		  flag3 = 1;
  		}
  		else if( flag1 == 0x52 )  //�ز����ۿ�
  		{
				loop7:
  		  Order_SztDeductAgain(money);
				delay_us(15);
				temp = 0xff;
				temp = ManageSztDeductAgain();
				if( temp == R_SztDeductAgain )
				{
					flag2 = ManageSztDeductAgain();
					if( flag2 == 1)
					{
						flag3 = 1;
					}
				}
  		}
	  }
		if( flag3 == 1) 
		{
	    if( (SztReductInf.BeginMoney - SztReductInf.EndMoney ) == money )
			{
				VariableChagelong(amountof_consumption,money);
				VariableChagelong(cardbalence_after,SztReductInf.EndMoney);	
				//��ʾ����ͨ�ۿ�������SztReductInf.BeginMoney���ۿ�ǰ����SztReductInf.EndMoney���ۿ����
				delay_ms(1000);
				return 1;
			}
			else
		  {
			  //�ٴ��鿨
				Order_Gpboc_ReadCard();
				temp = 0xff;
				temp = ManageReOrderType();
				if(temp == R_GpbocReadCard)
				{
					flag4 = ManageGpbocReadCard();
				}
				if(flag4 == 0x01)
				{
					EndMoney= UpReadCardData.SztMoney;
				}
			}
	  }
		else
		{
			tempcount++;
			if(tempcount <5)
			{
				goto loop7;
			}
			
		}
		if( (flag4 == 0x01) &&(flag3 == 0x01))
		{
			TrueMoney = FristMoney - EndMoney;
			if( TrueMoney == money)
			{
				endflag = 1;
			}
		}
  }
	else
	{
		if(cardbalence_cancel_flag)
		{
			cardbalence_cancel_flag=false;
			return 0;
		}
    cnt_t++;
		DealSeriAceptData();
	  if(cnt_t<15) goto loop6;		
	}
  return endflag; 
}





/*����ͨ�ϴ����ݣ��ϴ��ɹ�����1��ʧ�ܷ���0*/
uint8_t SztAutoSend(void)
{
  uint8_t temp =0xff;
  uint8_t flag1 = 0x00;

  Order_SztAutoSendData();
  temp = ManageReOrderType();
  if( temp == R_SztCheck_In)
  	{
  	  flag1 = ManageSztAutoManage();	  
  	}

  return flag1;
}



/*����ͨǩ��*/
bool Szt_GpbocAutoCheckIn(void)
{
  uint8_t Szt_DiverInitFlag =0;
  uint8_t Gpboc_DiverInitFlag =0;
  uint16_t check_cont=0;
  while(1)	 
  {
    if( Gpboc_DiverInitFlag ==0)
 	  {
 	    delay_us(500);
 	    Gpboc_DiverInitFlag = Gpboc_CheckIn();
 	  }
	  if( Gpboc_DiverInitFlag == 1)
	  {
			check_cont=0;
	    break;
	  }
		else 
		{
			check_cont++;
			if(check_cont>1000)
				return false;
		}
  }  
  while(1)	 
  {
    if( Szt_DiverInitFlag ==0)
 	  {
 	    delay_us(500);
 	    Szt_DiverInitFlag = Szt_CheckIn();
 	  }
	  if(Szt_DiverInitFlag == 1)
	  {
		  break;
	  }
		else 
		{
			check_cont++;
			if(check_cont>1000)
				return false;
		}
  }
	return true;
}

/*һ��2λ��BCD��ת����һ��ʮ������*/
uint8_t  BCDtoDec(uint8_t bcd)  
{  
     uint8_t i, tmp; 
   
     for(i=0; i<2; i++)  
     {  
        tmp = ((bcd>>4) & 0x0F)*10 + (bcd & 0x0F); 
     }  
   
     return tmp;  
}  
   


/*һ��2λ��ʮ������ת����һ��BCD��*/ 
uint8_t DectoBCD(uint8_t Dec)  
{  
     uint8_t temp;

	 temp = Dec/10*16 + Dec%10;
   
     return temp;  
}  






