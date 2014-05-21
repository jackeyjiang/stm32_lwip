#include "stm32f4xx.h"
#include "PVD.h"

void PVD_Configuration(void) 
{ 	
  EXTI_InitTypeDef EXTI_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
//	EXTI_DeInit(); 											Ϊʲô�Ұ������ȥ���ˡ��ͽ������ⲿ�ж����أ�����
	EXTI_StructInit(&EXTI_InitStructure); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line16; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);      // Configure EXTI Line16 to generate an interrupt 
	
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Configure one bit for preemption priority 
 	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);          // Enable the PVD Interrupt 
	EXTI_ClearITPendingBit(EXTI_Line16 );
	PWR_PVDLevelConfig(PWR_PVDLevel_5); 
  PWR_PVDCmd(ENABLE);
	
} 

/********************************
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
�еĳ�ʼ��ֵ�����������Ҫ�����޸ģ�����ϸ�����£�
    EXTI_Trigger_Rising --- ��ʾ��ѹ�Ӹߵ�ѹ�½��������趨�ĵ�ѹ��ֵ�����жϣ�
    EXTI_Trigger_Falling --- ��ʾ��ѹ�ӵ͵�ѹ�����������趨�ĵ�ѹ��ֵ�����жϣ�
    EXTI_Trigger_Rising_Falling --- ��ʾ��ѹ�Ӹߵ�ѹ�½��������趨�ĵ�ѹ��ֵ����ӵ͵�ѹ�����������趨�ĵ�ѹ��ֵ�����жϡ�
 

*********************************/

