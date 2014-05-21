 #include "stm32f4xx.h"
#include "timer2.h"



/*******************************************************************************
 * ��������:TIM2_Configuration                                                                    
 * ��    ��:TIM2��ʼ������   5ms�ж�һ��                                                                
 *                                                                               
 * ��    ��:����Ƶ��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/


void TIM2_Init(void)				   //���ڲɼ�ADC�õĶ�ʱ��
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//����ʱ��ʹ��  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;					
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ����
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = 10000-1;//��������װֵ
  	TIM_TimeBaseStructure.TIM_Prescaler = 4199;
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ʽ
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM2, (4200-1), TIM_PSCReloadMode_Immediate);//����16λʱ�ӷ�Ƶϵ��,��������ģʽ
	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
  	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//�����ж�
  	TIM_Cmd(TIM2, DISABLE);
}

void OpenTIM2(void)
{
   TIM_Cmd(TIM2, ENABLE);
}

void CloseTIM2(void)
{
   TIM_Cmd(TIM2, DISABLE);
}
