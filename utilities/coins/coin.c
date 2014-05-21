#include "bsp.h"
#include "stm32f4xx.h"
/*******************************************************************************
 ˵�������ļ�ΪͶ�һ�����ģ��
*******************************************************************************/

/*******************************************************************************
* Function Name  : InitCoins
* Description    :Ӳ�һ���ʼ����Ͷ�ң�;������Դ���ƿ�
* Input          : void
* Output         : void						 
* Return         : void
*******************************************************************************/
void InitCoins(void)
{									 
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ���ṹ��
	  NVIC_InitTypeDef NVIC_InitStructure;//�ж�������ʼ���ṹ��
	  EXTI_InitTypeDef EXTI_InitStructure;//�ⲿ�жϳ�ʼ���ṹ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//����ʱ��ʹ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//���������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = 	GPIO_PuPd_UP ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//Ͷ�һ���Դ���ƽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = 	GPIO_PuPd_DOWN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource12); //�ж���
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line12);
}
/*******************************************************************************
* Function Name  : OpenCoinMachine
* Description    : ��Ͷ�һ���Դ
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
extern unsigned int   CoinsCount;
void OpenCoinMachine(void)		 
{
  GPIO_SetBits(GPIOE, GPIO_Pin_13);
  CoinsCount=0;
}

/*******************************************************************************
* Function Name  : OpenCoinMachine
* Description    : �ر�Ͷ�һ���Դ
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void CloseCoinMachine(void)	
{
  GPIO_ResetBits(GPIOE, GPIO_Pin_13);
  CoinsCount=0;
}
