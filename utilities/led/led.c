#include "led.h"
#include "stm32f4xx.h"

//LED��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO��ʼ���ṹ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);//����ʱ��ʹ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//����ʱ��ʹ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//��ӦGPIO����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//IO�ٶ�Ϊ100MHZ
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_14 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);	 
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//��ӦGPIO����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//IO�ٶ�Ϊ100MHZ
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//��ӦGPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//IO�ٶ�Ϊ100MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

 //   GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void LEDOn(Led_Def Led)//LED��
{
  	LED_PORT[Led]->BSRRL=LED_PIN[Led];
}

void LEDOff(Led_Def Led)//LED��
{
  	LED_PORT[Led]->BSRRH=LED_PIN[Led];  
}

void LEDTog(Led_Def Led)//LED״̬��ת
{
  	LED_PORT[Led]->ODR^=LED_PIN[Led];
}

