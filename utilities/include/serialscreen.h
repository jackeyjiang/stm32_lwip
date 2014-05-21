#ifndef __serialscreen___
#define  __serialscreen___
#include "stdint.h"
/*֡��һЩ�ֽڶ���*/
#define FH0 0x5A 
#define FH1 0xA5
#define RD_LEN 0x00
#define ADR 0x00

#define Logo_interface        0x00  /*logo����*/
#define Err_interface         0x01  /*������ʾ����*/
#define Menu_interface        0x02  /*ѡ�ͽ���*/
#define Meal1st_interface     0x04  /*���ܲ������������*/
#define Meal2rd_interface     0x07  /*�㹽�����������*/
#define Meal3ns_interface     0x0A /*��Ƥ��Ѽ������*/
#define Meal4th_interface     0x0D /*�������������*/
#define Acount_interface      0x10 /*�������*/
#define TicketPrint_interface 0x13 /*СƱ��ӡ����*/
#define Mealout_interface     0x15 /*���ͽ�����ʾ*/
#define Password_interface    0x16 /*�����������*/
#define MealSet_interface     0x18 /*�Ų����ý���*/
#define MealInput_interface   0x1A /*�Ų�����ѡ��*/
#define TemperatureSet_interface 0x1C  /*�¶����ý���*/
#define Data_synchronization  0x1E  /*����ͬ��*/
#define Coinset_interface     0x22  /*Ӳ������*/
#define Cardbalence_interface 0x25  /*ˢ������*/
#define UserAbonamalRecord_interface 0x27 /*�쳣�û���¼����*/
#define HardwareInit_interface         0x29 /*Ӳ����ʼ������*/
#define OnlymachieInit_interface       0x2A /*��е�ֳ�ʼ������*/
#define SignInFunction_interface       0x2B /*����ǩ������*/
#define Szt_GpbocAutoCheckIn_interface 0x2C /*����ͨǩ������*/

/*�Ĵ����ռ䶨��*/
#define Version 0x00  /*DGUS�汾��*/
#define LED_NOW 0X01  /*LED���ȿ��ƼĴ�����0x00~0x40*/
#define BZ_IME  0X02  /*���������п��ƼĴ�������λ10ms*/
#define PIC_ID  0x03  /*2�ֽ� ������ǰ��ʾҳ��ID д���л���ָ��ҳ��(ҳ�����)*/
#define TP_Flag 0x05  /*0x5a=�����������и��£���������������δ����*/
#define TP_Status 0x06  /*0x01=��һ�ΰ��� 0x03һֱ��ѹ�� 0x02=̧�� ����=��Ч*/
#define TP_Position 0x07 /*4�ֽ� ��������ѹ����λ�ã�X_H:L Y_H:L*/
#define TPC_Enable 0x0B /*0x00=���ز����� ����=����*/

#define ScreenEnable  0x01 
#define ScreenDisable 0x00

/*ʣ���Ʒ������ַ*/
#define meat     0x0021 /*���ܲ�����*/
#define chicken	 0x0022 /*�㹽����*/
#define duck     0x0023 /*��Ƥ��Ѽ*/
#define fish     0x0024 /*�������*/

/*װ��ѡ��,1:����,2:���,3:ȷ��,4:ȡ��,5:����*/
#define mealinput_choose  0x001A

/*������������ַ*/
#define err_num  0x0007

/*ϵͳ����,1:�¶�����;2:��Ʒ����*/
#define admin_set 0x0006

/*���볤���ı���ʾ*/
#define password_show 0x0005 

/*������ʣ*/
#define erro_num    0x4D00 

/*�������������ַ*/
#define password 0x0004 

/*Ʊ�ݴ�ӡ������ַ,1:��;2:��*/
#define bill_print     0x0003 

/*֧����ʽ������ַ,1:�ֽ�֧��;2:���п�֧��;3:����֧ͨ��*/
#define payment_method  0x0002

/*ѡ�ͽ���ѡ�������ַ:��������ѡ��*/
#define mealcnt_choose  0x0001

/*������ѡ�ͽ���*/
#define meal_choose  0x0000

/*�¶����ñ���*/
#define temprature_set 0x001F

/*ʵʱ�¶���ʾ����*/
#define current_temprature 0x0020

/*��������ʾ*/
#define  payment_coin  0x0025 //Ӳ��֧�����
#define  payment_bill  0x0026 //ֽ��֧�����
#define  payment_card  0x0027 //ˢ��֧�����

/*�ȴ�ʱ��*/
#define  wait_payfor   0x0028 //����ȴ�ʱ��

/*���ͽ������*/
#define  mealout_totle  0x002A    //��Ʒ��������
#define  mealout_already  0x002B  //��Ʒ�ѳ�������

/*���ﳵ��ʾ*/
#define column1st_name  0x4100  //��һ����Ʒ��
#define column1st_count 0x0042  //��һ����Ʒ����
#define column1st_cost  0x0043  //��һ����Ʒ�ܼ�
#define column2nd_name  0x4200  //�ڶ�����Ʒ��
#define column2nd_count 0x0045  //�ڶ�����Ʒ����
#define column2nd_cost  0x0046  //�ڶ�����Ʒ�ܼ�
#define column3rd_name  0x4300  //��������Ʒ��
#define column3rd_count 0x0048  //��������Ʒ����
#define column3rd_cost  0x0049  //��������Ʒ�ܼ�
#define column4th_name  0x4400  //��������Ʒ��
#define column4th_count 0x004B  //��������Ʒ����
#define column4th_cost  0x004C  //��������Ʒ�ܼ�

#define mealtotoal_cost 0x004F  //�ܼ�

/*��ҳ��Ʒ������ʾ*/
//���ܲ���������ѡ�����
#define meat_cnt        0x0030 
#define meat_prince     0x0031
#define meat_cost       0x0032
//�㹽��������ѡ�����
#define chicken_cnt     0x0033
#define chicken_prince  0x0034
#define chicken_cost    0x0035
//��Ƥ��Ѽ����ѡ�����
#define duck_cnt        0x0036
#define duck_prince     0x0037
#define duck_cost       0x0038
//�����������ѡ�����
#define fish_cnt        0x0039
#define fish_prince     0x003A
#define fish_cost       0X003B
//ѡ�͵���ʱ
#define count_dowm      0x003F
//��Ʒ�۸�
/*��Ʒװ�����*/
#define row_1st   0x0011 
#define row_2nd   0x0012
#define row_3rd   0x0013
#define floor_num 0x0014
#define meal_num  0x0015

//�˱ұ���
#define coins_back  0x0050 //�˱Ҽ���
#define coins_in    0x0051 //�˱һ��ڵ�Ӳ������
#define coins_key   0x0052 //�˱ҵİ���

//ˢ��������
#define cardbalence_before    0x0060  //ˢ��Ǯ���
#define amountof_consumption  0x0065 //�������ѽ��
#define cardbalence_after     0x006A  //ˢ�������

#define caedbalence_cancel    0x0053  //ˢ��ȡ������

//ͬ������
#define sync_column1st_name   0x4500
#define sync_column2nd_name   0x4600
#define sync_column3rd_name   0x4700
#define sync_column4th_name   0x4800
#define sync_column1st_number 0x4900
#define sync_column2nd_number 0x4A00
#define sync_column3rd_number 0x4B00  
#define sync_column4th_number 0X4C00

//�û������¼����
#define record_column1st_name   0x5000 //��һ������
#define record_column2nd_name   0x5100 //�ڶ�������
#define record_column3rd_name   0x5200 //����������
#define record_column4th_name   0x5300 //����������

#define record_column1st_cnt_t  0x5400 //��һ����Ʒ����
#define record_column2nd_cnt_t  0x5500 //�ڶ�����Ʒ������
#define record_column3rd_cnt_t  0x5600 //��������Ʒ������
#define record_column4th_cnt_t  0x5700 //��������Ʒ������

#define record_column1st_cnt    0x5800 //��һ����Ʒ�ѳ�������
#define record_column2nd_cnt    0x5900 //�ڶ�����Ʒ�ѳ�����
#define record_column3rd_cnt    0x5A00 //��������Ʒ�ѳ�����
#define record_column4th_cnt    0x5B00 //��������Ʒ�ѳ�����

#define record_UserActPayAlready  0x5C00 //�û��Ѹ���
#define record_UserActPayBack     0x5D00 //�û�����

#define record_clear  0x0054/*�����¼����*/

extern const char price_1st;
extern const char price_2nd;
extern const char price_3rd;
extern const char price_4th;
extern bool cardbalence_cancel_flag;

void ClearUserBuffer(void);
void PageChange(char page);
void ScreenControl(char cmd);
void DispLeftMeal(void);
void DealSeriAceptData(void);
void VariableChage(uint16_t Variable,uint16_t Value);	
void DisplayAbnormal(char *abnomal_code);
void VariableChagelong (uint16_t Variable,uint32_t Value);		

#endif
