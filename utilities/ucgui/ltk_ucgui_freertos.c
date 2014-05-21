/**
  ******************************************************************************
  * �ļ�: ltk_ucgui_freertos.c
  * ����: LeiTek (leitek.taobao.com)
  * �汾: V1.0.0
  * ����: �����ֲ�˲���ϵͳ����ֲ uCGUI ��Ҫ����ĺ���
  ******************************************************************************
  *
  *                  ��Ȩ���� (C), LeiTek (leitek.taobao.com)
  *                                www.leitek.com
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "GUI_Private.h"
#include "ltk_systick.h"
#include "GUI_Private.H"
#include "ltk_ucgui_freertos.h"


/* GUI mutex, ���ڱ����ٽ���Դ */
static xSemaphoreHandle disp_sem;
/* GUI semaphore, ����ͬ������ */
static xSemaphoreHandle disp_binary_sem;

xSemaphoreHandle binary_sem_ts;

/**
  * ����: ��ʼ�� OS�������� GUI mutex �ʹ����� binary semaphore
  * ����: ��
  * ����: ��
  */
void  GUI_X_InitOS (void)
{
    disp_sem = xSemaphoreCreateMutex();
    vSemaphoreCreateBinary( disp_binary_sem );
    
    vSemaphoreCreateBinary(binary_sem_ts);
}

/**
  * ����: ���� GUI�����ڱ��� GUI �ٽ���Դ
  * ����: ��
  * ����: ��
  */
void  GUI_X_Lock (void)
{
    xSemaphoreTake(disp_sem, portMAX_DELAY);
}

/**
  * ����: ���� GUI�����ڱ��� GUI �ٽ���Դ
  * ����: ��
  * ����: ��
  */
void  GUI_X_Unlock (void)
{ 
    xSemaphoreGive(disp_sem);
}

/**
  * ����: �õ� task �� id
  * ����: ��
  * ����: task �� id
  */
U32  GUI_X_GetTaskId (void) 
{
    return (uint32_t)uxTaskGetTaskNumber(NULL);
}

/**
  * ����: �ȴ��¼�����������ͬ������
  * ����: ��
  * ����: ��
  */
void GUI_X_WaitEvent(void)
{
    xSemaphoreTake(disp_binary_sem, portMAX_DELAY);
}

/**
  * ����: ֪ͨ�¼��Ѿ�����������ͬ������
  * ����: ��
  * ����: ��
  */
void GUI_X_SignalEvent(void)
{
    xSemaphoreGive(disp_binary_sem);
}

/****************************** leitek.taobao.com *****************************/
