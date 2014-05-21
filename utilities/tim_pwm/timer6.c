#include "stm32f4xx.h"
#include "timer6.h" 
  
  /***********************************************
 ���ڶ�ʱʱ�䡣
 *************************************************/
//ͨ�ö�ʱ��3��ʼ��,��84MHz��TIM3ʱ��ԴΪ168MHz/2������TIM_scale��Ƶ����Ϊ����ʱ��
void TIM7_Init(void)//TIM_PeriodΪ16λ����	   //����1�����ж�һ��
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//����ʱ��ʹ��  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;					
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ����
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = 19999;//��������װֵ
  	TIM_TimeBaseStructure.TIM_Prescaler = 4199;
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ʽ
  	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM7, (4200-1), TIM_PSCReloadMode_Immediate);//����16λʱ�ӷ�Ƶϵ��,��������ģʽ
	  TIM_ClearFlag(TIM7, TIM_FLAG_Update);							    		/* �������жϱ�־ */
  	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//�����ж�
  	TIM_Cmd(TIM7, DISABLE);
}

void OpenTIM7(void)
{
   TIM_Cmd(TIM7, ENABLE);
}

void CloseTIM7(void)
{
   TIM_Cmd(TIM7, DISABLE);
}
