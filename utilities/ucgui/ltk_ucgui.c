/**
  ******************************************************************************
  * �ļ�: ltk_ucgui.c
  * ����: LeiTek (leitek.taobao.com)
  * �汾: V1.0.0
  * ����: ��ֲ uCGUI ��Ҫ����ĺ���
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
#ifdef LTK_FREERTOS
#include "FreeRTOS.h"
#endif

/**
  * ����: �պ���
  * ����: ��
  * ����: ��
  */
void GUI_X_Init (void) 
{
}

/**
  * ����: �ӳ�һ��ʱ��
  * ����: ��Ҫ�ӳٵ�ʱ��
  * ����: ��
  */
void  GUI_X_Delay (int period) 
{
    ltk_systick_delay(period);
}

/**
  * ����: �պ���
  * ����: ��
  * ����: ��
  */
void GUI_X_ExecIdle (void) 
{
}

/**
  * ����: �õ���ǰ��ʱ��
  * ����: ��
  * ����: ��ǰʱ��
  */
int  GUI_X_GetTime (void) 
{
    #if defined LTK_FREERTOS
    return ((int)xTaskGetTickCount());
    #elif (defined LTK_UCOS)
    OS_ERR  *p_err = OS_ERR_NONE;
    return ((int)OSTimeGet(p_err));
    #else
    return ((int)ltk_get_systick_timer());
    #endif
}

/**
  * ����: �պ���
  * ����: *s
  * ����: ��
  */
void GUI_X_Log(const char *s) 
{
    /* ֻ��Ϊ��ȥ��ĳЩ�������ľ�����Ϣ��û��ʵ������ */
    GUI_USE_PARA(s); 
}

/**
  * ����: �պ���
  * ����: *s
  * ����: ��
  */
void GUI_X_Warn(const char *s) 
{
    /* ֻ��Ϊ��ȥ��ĳЩ�������ľ�����Ϣ��û��ʵ������ */
    GUI_USE_PARA(s); 
}

/**
  * ����: �պ���
  * ����: *s
  * ����: ��
  */
void GUI_X_ErrorOut(const char *s)
{
    /* ֻ��Ϊ��ȥ��ĳЩ�������ľ�����Ϣ��û��ʵ������ */
    GUI_USE_PARA(s); 
}

/****************************** leitek.taobao.com *****************************/

