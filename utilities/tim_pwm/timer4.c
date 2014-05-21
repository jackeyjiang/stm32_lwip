#include "stm32f4xx.h"
#include "timer4.h"

void TIM4_Init(void)//TIM_PeriodΪ16λ����	   //����1�����ж�һ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//����ʱ��ʹ��  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;					
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//��ռ���ȼ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);					
	TIM_TimeBaseStructure.TIM_Period = 19999;//��������װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 4199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM4, (4200-1), TIM_PSCReloadMode_Immediate);//����16λʱ�ӷ�Ƶϵ��,��������ģʽ
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//�����ж�
	TIM_Cmd(TIM4, DISABLE);
}

void OpenTIM4(void)
{
  TIM_Cmd(TIM4, ENABLE);	
}

void CloseTIM4(void)
{
  TIM_Cmd(TIM4, DISABLE);
}

