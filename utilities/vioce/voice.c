#include "vioce.h"
#include "stm32f4xx.h"
#include "delay.h"

#define SPI_CS_H    GPIO_SetBits(GPIOD, GPIO_Pin_7)
#define SPI_CS_L    GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define SPI_SCK_H   GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define SPI_SCK_L   GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define SPI_MOSI_H	GPIO_SetBits(GPIOB, GPIO_Pin_6) 
#define SPI_MOSI_L	GPIO_ResetBits(GPIOB, GPIO_Pin_6)  
#define SPI_MISO_R	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)  
/*******************************************************************************
* Function Name  : VoiceReadWriteByte
* Description    : wt588d
* Input          : uint8_t TxData
* Output         : uint8_t
* Return         : uint8_t
*******************************************************************************/
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void VoiceReadWriteByte(uint8_t TxData)
{
  uint8_t i; 
	SPI_CS_L;
	delay_ms(2); /*Ƭѡ�źű��ֵ͵�ƽ2ms*/
    for (i = 0; i < 8; i++)
    {
      SPI_SCK_L;
      if (TxData&0x80)      
      {
          SPI_MOSI_H;
      }
      else
      {
          SPI_MOSI_L;
      }
      TxData <<= 1; 
	  delay_us(300);
	  SPI_SCK_H; 
	  delay_us(300);
	 }
	 SPI_CS_H;  
 } 

void InitVoice(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;//GPIO��ʼ���ṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);//����ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;//��ӦGPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO�ٶ�Ϊ100MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//��ӦGPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//״̬Ϊ����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO�ٶ�Ϊ100MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO�ٶ�Ϊ100MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}
void PlayMusic( unsigned char Data)
{
   unsigned char SendDataLoop;
   GPIO_ResetBits(GPIOD,GPIO_Pin_7);
   delay_us(5000);
   for(SendDataLoop=0;SendDataLoop<8;SendDataLoop++)
   {
     GPIO_ResetBits(GPIOB,GPIO_Pin_8);
     if((Data&0x01)==1)
     {
	     GPIO_SetBits(GPIOB,GPIO_Pin_6);
     }
     else
     {
	     GPIO_ResetBits(GPIOB,GPIO_Pin_6);
     }
   Data>>=1;
   delay_us(300);
   GPIO_SetBits(GPIOB,GPIO_Pin_8);
   delay_us(300);
   }
   GPIO_SetBits(GPIOD,GPIO_Pin_7);
}
