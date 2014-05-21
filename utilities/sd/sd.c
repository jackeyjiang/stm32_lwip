#include "bsp.h"
/***************************************
Ӳ��spi�ӿ�
***************************************/
//
//void  SD_GPIO_Configuration(void)
//{
//
//	GPIO_InitTypeDef GPIO_InitStructure;
//  	SPI_InitTypeDef  SPI_InitStructure;
////	DMA_InitTypeDef DMA_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//
////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
//
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    /* Configure SPI2 pins: SCK, MISO and MOSI */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
// //	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //PB13/14/15����
//
//   GPIO_SetBits(GPIOA,GPIO_Pin_4);	      //������ӵ�
// //   GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  SPI_Init(SPI2, &SPI_InitStructure);
//  SPI_Cmd(SPI2, ENABLE);
//
//}
//
//uint8_t SD_SPI_ReadWriteByte(uint8_t TxData)
//{
//
//	uint8_t retry=0;
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}
//	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
//	retry=0;
//
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
//		{
//		retry++;
//		if(retry>200)return ;
//		}
//	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
//}

/*************************************************************************
һ����ģ��SPI	 start
**************************************************************************/
void SD_GPIO_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;	   //CS
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;	   //MISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_OUT;
// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 	;   //SCLK
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;	   //MOSI
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_IN;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 /******************************************************************
 - ������������ʱ����
 - ����ģ�飺��������ģ��
 - �������ԣ��ⲿ���û��ɵ���
 - ����˵����time:timeֵ��������ʱ��ʱ�䳤��
 - ����˵������
 - ע��.....
 ******************************************************************/
unsigned char is_init;  //�ڳ�ʼ����ʱ�����ô˱���Ϊ1,ͬ�����ݴ��䣨SPI�������

#define DELAY_TIME 1000 //SD���ĸ�λ���ʼ��ʱSPI����ʱ����������ʵ�������޸���ֵ����������SD����λ���ʼ��ʧ��

void delay(unsigned int time)
{
 while(time--);
}

void SD_spi_write(unsigned char x)
{
 unsigned char i;
 for(i=0;i<8;i++)
 {
	 ResetSCLK ;
	 if(x&0x80)
	 {
     GPIOB->BSRRL = GPIO_Pin_5;
	 }
	 else
	 {
     GPIOB->BSRRH = GPIO_Pin_5;
	 }
	 if(is_init) delay(DELAY_TIME);
	 SetSCLK 	;
   x <<= 1 ;
 }
}

/******************************************************************
 - ����������IOģ��SPI����ȡһ���ֽ�
 - ����ģ�飺SD/SDHCģ��
 - �������ԣ��ڲ�
 - ����˵������
 - ����˵�������ض������ֽ�
 ******************************************************************/

unsigned char SD_spi_read() //SPI��һ���ֽ�
{
  unsigned char i,temp=0;

  for(i=0;i<8;i++)
  {
    SetSCLK ;
    if(is_init) delay(DELAY_TIME);
    ResetSCLK ;
    if(is_init) delay(DELAY_TIME);
    if(GPIOA->IDR & GPIO_Pin_6)
    {
      temp|=(0x80>>i);
    }
  }
  return (temp);
}

/*************************************************************************
������ģ��SPI	 end
**************************************************************************/
//�ж��Ƿ�����
u8 isRunNian(u16 y)
{
  return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) ? 1 : 0;
}
//����������õ��µ�����
u8 getDays(u16 y, u8 m)
{
  u8 days = 0;
  switch(m)
  {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			days = 31; break;
		case 4:
		case 6:
		case 9:
		case 11:
			days = 30; break;
		case 2:
			days = isRunNian(y) ? 29 : 28; break;
		default:;
	}
	return days;
}


  /*******************************************************************************
 * ��������:itoa
 * ��    ��:��intת�����ַ���
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
char 	f_name[12];
void itoa(char *f_name, Struct_TD  a)
{
	 uint8_t i,j=0;
	 f_name[j++] = 2               ;
	 f_name[j++] = 0			         ;
	 f_name[j++] = a.Year     /	10 ;
	 f_name[j++] = a.Year     %	10 ;
	 f_name[j++] = a.Month    / 10;
	 f_name[j++] = a.Month    % 10;
	 f_name[j++] = a.Date     / 10;
	 f_name[j++] = a.Date     % 10;
//	 f_name[j++] = a.Hours    / 10;
//	 f_name[j++] = a.Hours    % 10;
//	 f_name[j++] = a.Minutes  / 10;
//	 f_name[j++] = a.Minutes  % 10;

	 for(i=0;i<8;i++)
	 f_name[i] += '0'; //��������ת��Ϊ�ַ�
	 f_name[j++]  = '.';
	 f_name[j++]  = 't';
	 f_name[j++]	= 'x';
	 f_name[j++]	= 't';
}
  /*******************************************************************************
 * ��������:itoa   ��intת�����ַ���
 * ��    ��:��
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
FATFS fs;
FRESULT res;
FILINFO fileInfo;
UINT br,bw;
UINT r=1;
DIR dir;
FIL fsrc;

void Fwriter(unsigned char *p)
{
	uint8_t sd_time;
	uint32_t rd;
	res = f_mount(0, &fs);
	if(res!=FR_OK)
	{
		return ;
	}
	for(sd_time=0;sd_time<20;sd_time++)
	{
    res = f_open(&fsrc,f_name,FA_OPEN_ALWAYS|FA_WRITE );
	//	res = f_open(&fsrc,"0:2013011209408.txt", FA_OPEN_ALWAYS|FA_WRITE);  //�ļ��������򴴽����ļ�
		if(res==FR_OK) break;
	}
	if(res!=FR_OK)
	{
		return ;
	}
	else
	{
		res = f_lseek(&fsrc,fsrc.fsize);
   	res = f_write(&fsrc,p,512, &rd);
	//	res = f_write(&fsrc,"С��ͯЬ��ã�����\r\n",512, &rd);
		f_sync(&fsrc)	;
	  if(res!=FR_OK)
	  {
	    f_close(&fsrc);
	    f_mount(0, NULL);
	    return;
	  }
	  else
	  {
	    f_close(&fsrc);
	    f_mount(0, NULL);
	  }
  }
}


  /*******************************************************************************
 * ��������: Fread
 * ��    ��: �ӿ��ж�ȡ����
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
static uint32_t  Index = 0;
char  Buffer[256] = {0};
char  Fread(unsigned char *p)
{
		uint8_t   sd_time;
    uint32_t  rd;
	  uint16_t  Lenght = 0 ;
	  uint32_t  indexflag = 0;
	  res = f_mount(0, &fs);
		if(res!=FR_OK)
		{
			return 0;
		}
    res = f_open(&fsrc,f_name,FA_OPEN_EXISTING|FA_READ |FA_WRITE);  //Ĭ�ϴ�һ���ļ�    
		if(res!=FR_OK)
		{
		  return  1; //���ش���
		}
		else
		{
		  // res = f_lseek(&fsrc,fsrc.fsize);
		  res = f_lseek(&fsrc,Index); //ָ��ָ���Index���ֽ�
			//	printf("Index = %d \r\n",res);
		  res = f_read(&fsrc,p,512, &rd);//��ȡ512�ֽڵ����ݵ�Buffer,rd �洢���Ƕ������ֽ���
			//	printf("Index = %d \r\n",res);
//				if(1)
//				//if(Resend(Buffer,Lenght) == 0x00)
//				{
//					indexflag = Index+1021 ;
//					res = f_lseek(&fsrc,indexflag);//ƫ��Index+1021
//          res = f_write(&fsrc,"Yes",3, &rd); //д"Yes"
//				  f_close(&fsrc);
//        }
//				else
//				{
//					*p =  0x01 ;
//					res = f_lseek(&fsrc,1000);
//					res = f_write(&fsrc,"Uncomplete",10, &rd);
//					f_close(&fsrc);
//        }
      }
		  Index += 512 ; //ƫ��512
		  // printf("Index = %d \r\n",Index);
	    if(res!=FR_OK ||fsrc.fsize < Index ) //fsrc.fsize���ļ���С�������ж��ļ���С֮ǰ��if(res!=FR_OK ||fsrc.fsize <=Index )
	    {
//	      f_close(&fsrc);????
//      f_mount(0, NULL);
	      return 1;
	    }
	    else
	    {
//	      f_close(&fsrc); ?????
//	    f_mount(0, NULL);
		    return 0 ;
	    }
 }

  /*******************************************************************************
 * ��������:WriteDatatoSD
 * ��    ��:����Ϣд��sd��
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
void WriteDatatoSD(unsigned char *data)
{
	RTC_TimeShow();
	itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
	Fwriter(data);					      //��������ˡ�
}


  /*******************************************************************************
 * ��������:ReadDatatoBuffer
 * ��    ��:��SD���ж�ȡ����
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
bool ReadDatatoBuffer(void)
{
  unsigned char DelteFlag = 0 ;//��������Ƿ�ȫ���ϴ������ݵĺ���
	unsigned int  Times  = 0  ;
	uint32_t  indexflag = 0;
	for(Times = 0 ;Times <1000 ;Times++)
	{
	  if(Fread(ReadSdBuff)== 0x01)					      //��������ˡ??
       break;
		else 
		{
			SearchSeparator(ReadBuf,ReadSdBuff,18);
			if(ReadBuf[0]=='n')
			{
				if(TakeMealsFun1(ReadSdBuff)==0) //�ڵ�ʮ�������ź�дY 255-162=93
				{
					indexflag = Index-512+167; //����Ҫ����120-26  256-120-26=
					res = f_lseek(&fsrc,indexflag);//ƫ��Index+1021
					res = f_write(&fsrc,"y",1, &bw); //д"Y"
				  f_close(&fsrc);
        }				 
				else
				{
					f_close(&fsrc);
					printf("send to host is erro");	
					return false;
				}					
			}
		}				 
  }
	return true;
}


  /*******************************************************************************
 * ��������:SendtoServce
 * ��    ��:�ϵ�ʱ�̼������     �����ѻ�����7�졣����
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
void SendtoServce(void)
{
//20140211.txt
	unsigned char i    = 0;
	RTC_TimeShow();
	for(i=0;i<7;i++)
	{
		itoa(f_name,TimeDate);	  //�ļ���ΪTimeDate
//	printf("%s\r\n",f_name);
	 	if(TimeDate.Date == 1)
		{
			TimeDate.Date  = getDays(2000+TimeDate.Year,TimeDate.Month);
			if(TimeDate.Month == 1)
		  TimeDate.Month   = 12;
    }
		 //�������� ???
		ReadDatatoBuffer();
		TimeDate.Date -- ;
  }
}

  /*******************************************************************************
 * ��������:File_delete
 * ��    ��:�ϵ�ʱ��ɾ��ǰ2���µļ�¼
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:��
 * �޸�����:2013��4��20��
 *******************************************************************************/
void File_delete(void)
{
	 unsigned char i = 0 ,j=0,days = 0;
	 RTC_TimeShow();
 //  TimeDate.Month  = TimeDate.Month % 10 - 2 ;  //ɾ��ǰ�����µļ�¼
	 itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
//	 printf("%s\r\n",f_name);
	 if(TimeDate.Month == 0x01)
	 {
		 TimeDate.Month = 11;
		 TimeDate.Year -=1 ;
	 }
	 if(TimeDate.Month == 0x02)
	 {
		 TimeDate.Month = 12;
	   TimeDate.Year -=1 ;
	 }
// TimeDate.Month =TimeDate.Month>2 ? TimeDate.Month-2: 12-TimeDate.Month;
	 for(i=0;i<2;i++)
	 {
			res = f_mount(0, &fs);
			if(res!=FR_OK)
			{
			return ;
			}
//	  TimeDate.Month= GetageMonth(TimeDate.Month);
	    itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
		  TimeDate.Date  = getDays(2000+TimeDate.Year,TimeDate.Month);
		  days = TimeDate.Date ;
		  for(j=0;j<days;j++)
		  {
				itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
//				printf("%s\r\n",f_name);
				f_unlink(f_name);  //
				TimeDate.Date -- ;
      }
			TimeDate.Month-- ;
   }
}

void DataRecord(void)
{
   char sd_time=0;
	 RTC_TimeShow();
	 if(UserAct.MealCnt_1st_t>0)
	 {
     UserAct.MealID = 0x01;
		 MealArr(UserAct.MealID);
		 itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
     Sd_Write('N',Success);
	 }
	 if(UserAct.MealCnt_2nd_t>0)
	 {
     UserAct.MealID = 0x02;
		 MealArr(UserAct.MealID);
		 itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
     Sd_Write('N',Success);
	 }
	 if(UserAct.MealCnt_3rd_t>0)
	 {
     UserAct.MealID = 0x03;
		 MealArr(UserAct.MealID);
		 itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
     Sd_Write('N',Success);
	 }
	 if(UserAct.MealCnt_4th_t>0)
	 {
     UserAct.MealID = 0x04;
		 MealArr(UserAct.MealID);
		 itoa(f_name,TimeDate);	  //��ʱ��ת�����ַ�
     Sd_Write('N',Success);
	 }
}


void HextoChar(char *destbuff,char *buffer)
{
  char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i=0,j=0,k=0;
  char temp=0;
  while(1)
  {
   if(buffer[i]==',')
   {
      destbuff[j++]=buffer[i];
		  temp++;
   }
   else if((buffer[i]=='n')||(buffer[i]=='y'))
   {
		 if(temp!=9)
		 {
			destbuff[j++]= buffer[i]; 
		  destbuff[j]='\r';
			break;
		 }
   }   
   else
   {
		 if(temp!=9)
		 {
			 destbuff[j++] = index[(buffer[i]&0xf0)>>4];
			 destbuff[j++] = index[(buffer[i]&0x0f)];
		 }
		 else
		   destbuff[j++]=buffer[i];
   }
   i++;
  }
//  destbuff[j++]='\r'; //�����Խ�β���л���
//  destbuff[j]='\n';	
}


char Send_Buf[103] ={0};
char Rec_Buf[512]={0};
void Sd_Write(char erro_flag,char takeout_flag)
{
   uint16_t CmdLenght =0,i=0,j=0;
   long CRCValue=0;
  	 /*ˮ����++*/
	 Send_Buf[0] =	0x02 ;
	 Send_Buf[1] =	0x80 ;
	 Send_Buf[2] =	0x00 ;
	 Send_Buf[3] =	0x00 ;
	 Send_Buf[4] =	0x00 ;
	 Send_Buf[5] =  ','  ;
	 CmdLenght = 6 ;
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&TID[3],sizeof(TID)-3);	/*�ն˵�TID*/
	 GetBRWN();
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&BRWN[3],sizeof(BRWN)-3);  /*��ˮ��*/
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&BNO[3],sizeof(BNO)-3);	/*���κ�*/
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&DeviceArea[3],sizeof(DeviceArea)-3);  /*�ն�����������*/
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&DeviceAreaNO[3],sizeof(DeviceAreaNO)-3); /*�ն����ڵ�����*/
	 for(i=0;i<6;i++)
	 {
	   DealBalance[3+i] = CustomerSel.DealBalance[i] ;
	 }
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&DealBalance[3],sizeof(DealBalance)-3); /*���׽��(֧�����) */

	 for(i=0;i<4;i++)
	 {
	  //���︳ֵ��Ʒ��ID
	   MealID[3+i] = CustomerSel.MealID[i] ; //(10000020)
	 }
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&MealID[3],sizeof(MealID)-3); /*��ƷID*/

	 /*���︳ֵ��Ʒ��ID*/
	 MealNO[3] = CustomerSel.MealNo;
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&MealNO[3],sizeof(MealNO)-3); /*��Ʒ��������*/

	  //���︳ֵ��Ʒ������
	 for(j=0;j<20;j++)
	  MealName[3+j]=Meal[CustomerSel.MealName-1].MaelName[j];
    /*��Ʒ�����޷��úô���*/
	  
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&MealName[3],sizeof(MealName)-3);			  /*��Ʒ����*/
	 
	 /*���︳ֵ��Ʒ�ļ۸�*/
	 for(i=0;i<6;i++)
	 MealPrice[3+i] = CustomerSel.MealPrice[i] ;

	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&MealPrice[3],sizeof(MealPrice)-3);			/*��Ʒ�۸�*/
	/*��Ǯ�ķ�ʽ*/
	 PayType[3] = CustomerSel.PayType ;
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&PayType[3],sizeof(PayType)-3);				/*֧����ʽ*/
	 /*������*/
	 for(i=0;i<6;i++)
	 Change[3+i] = CustomerSel.Change[i] ;
	 CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&Change[3],sizeof(Change)-3); 			   /*������*/
	 /*ʣ���Ʒ����*/
	 for(i=0;i<2;i++)
	 {
	   RemainMealNum[3+i] = CustomerSel.RemainMealNum[i] ;
    //printf("CustomerSel.RemainMealNum[i]=%d\r\n",CustomerSel.RemainMealNum[i]);
	 }
	   CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&RemainMealNum[3],sizeof(RemainMealNum)-3);  /*ʣ���Ʒ����*/
	   TakeMealFlag[4]= takeout_flag;
	   CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&TakeMealFlag[3],sizeof(TakeMealFlag)-3); /*ȡ�ͱ��*/
	   CmdLenght +=mem_copy01(&Send_Buf[CmdLenght],&MAC[3],sizeof(MAC)-3);					  /*MAC*/
 //  if(UserAct.PayType == '2' )																		/* ��ʾ�����ˢ��*/
 //  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],STATUS.PacketData,STATUS.DataLength-17);				 /*��¼ˢ����Ϣ*/
 //  if(UserAct.PayType == '3')
 //  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],STATUS.PacketData,STATUS.DataLength-17);				 /*��¼ˢ����Ϣ*/
	 Send_Buf[CmdLenght] = 0x03  ;
	 CmdLenght+=0x03;
   /*��Ӧ --> ��λ*/
	 HL_IntToBuffer(CmdLenght-8,&Send_Buf[3]);
	 //CRCValue=GetCrc16(&Send_Buf[1],CmdLenght-4);
	 CRCValue=0x0000;
	 HL_IntToBuffer(CRCValue,&Send_Buf[CmdLenght-2]); 
	 Send_Buf[CmdLenght++]=',';
	 Send_Buf[CmdLenght]=erro_flag;
//	 Send_Buf[126]='\r';
//	 Send_Buf[127]='\n';
	 HextoChar(Rec_Buf,Send_Buf);
	 Fwriter(Rec_Buf);
	 memset(Send_Buf,0,100);
   memset(Rec_Buf,0,512);
}



char *myitoa(int num,char *str,int radix)
{     /* ������*/
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum; /* �м���� */
    int i=0,j,k;
	  char temp;
    /* ȷ��unum��ֵ */
    if(radix==10&&num<0) /* ʮ���Ƹ��� */
    {
    unum=(unsigned)-num;
    str[i++]='-';
    }
    else unum=(unsigned)num; /* ������� */
    /* ת�� */   
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    /* ���� */
    if(str[0]=='-') k=1; /* ʮ���Ƹ��� */
    else k=0;
    for(j=k;j<=(i-1)/2;j++)
    {
        temp=str[j];
        str[j] = str[i-1+k-j];
        str[i-1+k-j] = temp;
    }
    return str;
}
  /*******************************************************************************
 * ��������:SearchSeparator                                                                     
 * ��    ��:��ȡ��n ���������                                                      
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��19��                                                                    
 *******************************************************************************/ 
void SearchSeparator(char *dest,char *souce,int Separator)
{
	char temp=0;
	int SeparatorCnt=0;
	int i=0,j=0;
	while(1)
	{
		temp= souce[i];
		if(temp==',')//&&SeparatorCnt==Separator)
		{
			SeparatorCnt++;
		}
		else if(SeparatorCnt== Separator-1)
		{
			dest[j++]=temp;
			if(Separator==18)
				break;
		}
		if(SeparatorCnt==Separator)
		{
			break;
		}		
		i++;
	}
}

  /*******************************************************************************
 * ��������:SendDataToHost                                                                    
 * ��    ��:��ȡSD���е�����                                                     
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��19��                                                                    
 *******************************************************************************/ 
void SendDataToHost(void)
{
	
}