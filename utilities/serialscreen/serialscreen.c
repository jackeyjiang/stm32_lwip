 /**
  ******************************************************************************
  * �ļ�: serialscreen.c
  * ����: jackey
  * �汾: V1.0.0
  * ����: DWIN ������ͨ�ų���
  *  
  ******************************************************************************
	**/
#include <string.h>
#include "stm32f4xx.h"
#include "serialscreen.h"
#include "uart3.h"
#include "bsp.h"
#include "MsgHandle.h"

#define Null 0x00
typedef struct DispMeal
{
	char meal_id;
	char meal_cnt;
	char meal_cost;
}DispMealStuct;

DispMealStuct DispMeal[4]={0};

const char price_1st= 15;
const char price_2nd= 15;
const char price_3rd= 15;
const char price_4th= 15;

const char meat_name[12]={"���ܲ�����"};
const char chicken_name[10]={"�㹽����"};
const char duck_name[10]={"��Ƥ��Ѽ"};
const char fish_name[10]={"�������"};

/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRam
{
		int16_t	 	adress ;
		int8_t  	adr[2] ;
}myunion;

/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRamLong
{
		uint32_t	 	adress ;
		int8_t  	adr[4] ;
}myunion_long;


/*дָ���Ĵ���֡*/
const unsigned char RegisterWrite[7]={ 
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x04 , /*���� �������������*/
              0x80 , /*ָ����ַд�Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							0x00 ,0x00 /*����*/
							//0x00 , 0x00	/*CRCУ����β*/
							};

/*��ָ���Ĵ���֡*/
const unsigned char RegisterRead[6]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x03 , /*���� �������������*/
              0x81 ,  /*ָ����ַ���Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							RD_LEN , /*����*/
							//0x00 , 0x00	/*CRCУ����β*/	
							};
	
/*д�����洢��֡*/							
const unsigned char VariableWrite[8]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x00 , /*���� �������������*/	
							0x82 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							0x00 , 0x00   /*����*/
							//0x00 , 0x00  	/*CRCУ����β*/
							}; 

/*�������洢��֡*/	
const unsigned char VariableRead[7]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x04 , /*���� �������������*/
							0x83 , /*�ӱ����洢��ָ����ַ��ʼ����RD_LEN����������*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							RD_LEN , /*����*/
							//0x00 , 0x00  	/*CRCУ����β*/
							};	

const char ABC[20]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x05 , /*���� �������������*/	
							0x82 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x40 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							0xBA , 0xA0   /*����*/
              };	
#include "stdio.h"

char *mystrcat(char *dest, const char *src, char length)
{
  int i=0;
	for(i=0;i<length;i++)
	{
		dest[6+i] = src[i];
	}
  return dest;
}


 /*******************************************************************************
 * ��������:VariableChage                                                                    
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void VariableChage(uint16_t Variable,uint16_t Value)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 5;
		myunion.adress= Variable; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		myunion.adress= Value; 
		temp[6]= myunion.adr[1];
		temp[7]= myunion.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}							

 /*******************************************************************************
 * ��������:VariableChagelong                                                                    
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void VariableChagelong (uint16_t Variable,uint32_t Value)							
{
	  unsigned char temp[10]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 7;
		myunion.adress= Variable; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		myunion_long.adress= Value;
		temp[6]= myunion_long.adr[3];
		temp[7]= myunion_long.adr[2];					
		temp[8]= myunion_long.adr[1];
		temp[9]= myunion_long.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:RegisterChage                                                                   
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void RegisterChage(uint8_t Register,uint16_t Value)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));
		temp[3]= Register;
		myunion.adress=Value;
		temp[5]= myunion.adr[1];
		temp[6]= myunion.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}	

 /*******************************************************************************
 * ��������:RegisterChage                                                                   
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void ScreenControl(char cmd)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));
		temp[3]= TPC_Enable;
		temp[5]= cmd ;	//0x00 �ر�
		Uart3_Send(temp,sizeof(temp));	
}	

 /*******************************************************************************
 * ��������:TextDisp                                                                  
 * ��    ��:��ʾ�ַ�������,                                                          
 *                                                                               
 * ��    ��:meal_name                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void TextDisp(uint16_t meal_name)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_name; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_name)
		{
			case column1st_name:strcat(temp,meat_name);break;	
			case column2nd_name:strcat(temp,chicken_name);break;		
			case column3rd_name:strcat(temp,duck_name);break;	
			case column4th_name:strcat(temp,fish_name);break;
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:MealNameDisp                                                                  
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 15; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_name+floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,meat_name,10);break;	
			case 0x02:mystrcat(temp,chicken_name,8);break;		
			case 0x03:mystrcat(temp,duck_name,8);break;	
			case 0x04:mystrcat(temp,fish_name,8);break;
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:MealCntDisp                                                                 
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_count+floor*3; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= meal_cnt;
		Uart3_Sent(temp,sizeof(temp));	
}

 /*******************************************************************************
 * ��������:MealCostDisplay                                                                
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealCostDisplay(uint8_t meal_cost,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_cost+floor*3; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= meal_cost;
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:DisplayPassWord                                                                 
 * ��    ��:��ʾ���볤����*��ʾ                                                     
 *                                                                               
 * ��    ��:PassWordLen                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void DisplayPassWord(char PassWordLen)
{
	char i=0;
	char temp[20]={0};  //��Ŵ������ݵ���ʱ����
	memcpy(temp,VariableWrite,sizeof(VariableWrite));
	temp[2]= PassWordLen+ 4; //  0x83 00 00 ****** �����������ʾ���������
	myunion.adress= password_show; 
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
	for(i=0;i<PassWordLen;i++) temp[i+6]='*';
	Uart3_Sent(temp,sizeof(temp)); 
}	
 /*******************************************************************************
 * ��������:DisplayAbnormal                                                               
 * ��    ��:��ʾ������                                                   
 *                                                                               
 * ��    ��:abnomal_code �ַ���                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void DisplayAbnormal(char *abnomal_code)
{
	char i=0;
	char temp[20]={0};  //��Ŵ������ݵ���ʱ����
	memcpy(temp,VariableWrite,sizeof(VariableWrite));
	temp[2]= 7; //  ���ݳ���Ϊ4λ
	myunion.adress= erro_num; 
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
	for(i=0;i<4;i++) temp[i+6]=abnomal_code[i];
	Uart3_Sent(temp,sizeof(temp)); 	
}


 /*******************************************************************************
 * ��������:PageChange                                                                     
 * ��    ��:�����л�ͼƬ����                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	
char pageunitil= 0;
void PageChange(char page)
{
		unsigned char temp[7]={0};
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));	
		temp[4]=	PIC_ID;	
	  temp[6]=  page;
		pageunitil = page;
		Uart3_Send(temp,sizeof(temp));
}	

 /*******************************************************************************
 * ��������:DispLeftMeal                                                                     
 * ��    ��:�ڲ˵�ѡ�������ʾ����Ʒ��ʣ���                                                           
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	

void DispLeftMeal(void)
{
		uint8_t i;
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		for(i=0;i<4;i++){		
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 5;
		myunion.adress= meat+i; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		//�����ݽ�����䣬��Ҫ�õ�flash
		temp[7]= DefineMeal[i].MealCount;
		Uart3_Send(temp,sizeof(temp));	
		}
}
 /*******************************************************************************
 * ��������:CutDownDisp                                                                     
 * ��    ��:��ʾ����ʱ60-0                                                          
 *                                                                               
 * ��    ��:time                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	
void CutDownDisp(char time)
{
	  unsigned char temp[8]={0};
	  memcpy(temp,VariableWrite,sizeof(VariableWrite));
	  temp[2]= 5;
		myunion.adress= count_dowm; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= time;	
		Uart3_Send(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:MealCostDisp                                                                     
 * ��    ��:��ʾ��ǰ��ƷӦ���Ľ��                                                          
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
char GetMealPrice(char meal_type,char count)
{
	char price= 0;
  switch(meal_type)
	{
		case 0x01:
		{
			price= price_1st*count;
		}break;
		case 0x02:
		{
      price= price_2nd*count;
		}break;
		case 0x03:
		{
      price= price_3rd*count;
		}break;
		case 0x04:
		{
      price= price_4th*count;
		}break;
		default:break;
	}
	return price;
}

 /*******************************************************************************
 * ��������:MealCostDisp                                                                     
 * ��    ��:��ʾ��ǰ��ƷӦ���Ľ��                                                          
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void MealCostDisp(char meal_id,char meal_count)
{
	  unsigned char temp[8]={0};
	  memcpy(temp,VariableWrite,sizeof(VariableWrite));
	  temp[2]= 5;
		temp[5]= (0x32+(meal_id-1)*3); //Ǯ�ұ�����ַ
    temp[7]= GetMealPrice(meal_id,meal_count);	
		Uart3_Send(temp,sizeof(temp));		
}
 /*******************************************************************************
 * ��������:ClearUserBuffer                                                                     
 * ��    ��:����û�ѡ������                                                          
 *                                                                               
 * ��    ��:��                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void ClearUserBuffer(void)
{
	UserAct.MealCnt_1st=0;
	UserAct.MealCnt_1st_t=0;
	UserAct.MealCnt_2nd=0;
	UserAct.MealCnt_2nd_t=0;
	UserAct.MealCnt_3rd=0;
	UserAct.MealCnt_3rd_t=0;
	UserAct.MealCnt_4th=0;
	UserAct.MealCnt_4th_t=0;
	UserAct.MealCost_1st=0;
	UserAct.MealCost_2nd=0;
	UserAct.MealCost_3rd=0;
	UserAct.MealCost_4th=0;
	UserAct.MealID=0;
	UserAct.Meal_totoal=0;
	UserAct.PayShould=0;
	UserAct.PayType=0;
  UserAct.PayForCoins=0;
  UserAct.PayForCoins=0;           //�û�Ͷ���Ӳ����	
	UserAct.PayForBills=0;           //�û�Ͷ���ֽ����
	UserAct.PayForCards=0;           //�û�Ӧ��ˢ������
  UserAct.PayAlready=0;
}
 /*******************************************************************************
 * ��������:PutIntoShopCart                                                                     
 * ��    ��:��ѡ�����潫������빺�ﳵ֮��ĳ�����Ӧ                                                        
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void PutIntoShopCart(void)
{
	switch(UserAct.MealID)
	{
		case 0x01:
		{
			UserAct.MealCnt_1st= UserAct.MealCnt_1st_t;
			UserAct.MealCost_1st = GetMealPrice(UserAct.MealID,UserAct.MealCnt_1st);
		}break;
		case 0x02:
		{
			UserAct.MealCnt_2nd=UserAct.MealCnt_2nd_t;
			UserAct.MealCost_2nd = GetMealPrice(UserAct.MealID,UserAct.MealCnt_2nd);
		}break;
		case 0x03:
		{
			UserAct.MealCnt_3rd=UserAct.MealCnt_3rd_t;
			UserAct.MealCost_3rd = GetMealPrice(UserAct.MealID,UserAct.MealCnt_3rd);
		}break;
		case 0x04:
		{
			UserAct.MealCnt_4th=UserAct.MealCnt_4th_t;
			UserAct.MealCost_4th = GetMealPrice(UserAct.MealID,UserAct.MealCnt_4th);
		}break;
		default:break;	
	}
	//PageChange((UserAct.MealID-1)*3+6); //��֪������Ƿ������???
	UserAct.Meal_totoal= UserAct.MealCnt_4th+UserAct.MealCnt_3rd+UserAct.MealCnt_2nd+UserAct.MealCnt_1st;
}	

 /*******************************************************************************
 * ��������:SettleAccounts                                                                 
 * ��    ��:�����½��㰴ť��ĳ�������                                                                                                                        
 * ��    ��:��                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 
char Floor= 0; //��Ϊ����Ʒ����Ĳ���
void SettleAccounts(void)
{
	uint8_t tempcnt=0;
	//�����Ļ��ʾ����
  for(tempcnt=0;tempcnt<4;tempcnt++)
	{
		MealNameDisp(Null,tempcnt);
		MealCntDisp(Null,tempcnt);//��ʾ����
		MealCostDisplay(Null,tempcnt);//��ʾ���ܼ� �������ظ�		
	}
  for(Floor=0;Floor<4;Floor++)
	{
		DispMeal[Floor].meal_id= 0x00; //0x01Ϊ��һ����Ʒ��ID
		DispMeal[Floor].meal_cnt= 0x00; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= 0x00; //��Ʒ���ܼ۵ĸ�ֵ
	}
	Floor= 0;
	tempcnt=0; /*��������*/
  PageChange(Acount_interface);//�������
	//����������� current = payformoney
  //��ʾӲ����
	VariableChage(payment_coin,UserAct.PayForCoins);
  //��ʾֽ����
	VariableChage(payment_bill,UserAct.PayForBills);
  //ˢ����
	VariableChage(payment_card,UserAct.PayForCards);
  //��ʾ�ȴ�ʱ��
	VariableChage(wait_payfor,60);
	OpenTIM7();
	//������ʾ����
	Floor= 0; //ÿ�ν�һ�ν��˽������Ҫ����д������
	
	if(UserAct.MealCnt_1st>0)
	{
		DispMeal[Floor].meal_id= 0x01; //0x01Ϊ��һ����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_1st; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_1st; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}
	if(UserAct.MealCnt_2nd>0)
	{
		DispMeal[Floor].meal_id= 0x02; //0x02Ϊ�ڶ�����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_2nd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_2nd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}		
  if(UserAct.MealCnt_3rd>0)
	{
		DispMeal[Floor].meal_id= 0x03; //0x03Ϊ��������Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_3rd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_3rd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;		
	}		
	if(UserAct.MealCnt_4th>0)
	{
		DispMeal[Floor].meal_id= 0x04; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_4th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_4th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	//������ʾ�Ĳ��֣��ӵ�һ����ʼ��������
	for(tempcnt=0;tempcnt<Floor;tempcnt++)
	{
		MealNameDisp(DispMeal[tempcnt].meal_id,tempcnt);//��ʾ����  
		MealCntDisp(DispMeal[tempcnt].meal_cnt,tempcnt);//��ʾ����
		MealCostDisplay(DispMeal[tempcnt].meal_cost,tempcnt);//��ʾ���ܼ� �������ظ�
	}
	UserAct.PayShould= (UserAct.MealCost_1st+UserAct.MealCost_2nd+UserAct.MealCost_3rd+UserAct.MealCost_4th);
	VariableChage(mealtotoal_cost,UserAct.PayShould);
}

 /*******************************************************************************
 * ��������:SyncMealNameDisp                                                                  
 * ��    ��:����ͬ��ʱ��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void SyncMealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 15; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= sync_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,meat_name,10);break;	
			case 0x02:mystrcat(temp,chicken_name,8);break;		
			case 0x03:mystrcat(temp,duck_name,8);break;	
			case 0x04:mystrcat(temp,fish_name,8);break;
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:SyncMealCntDisp                                                                 
 * ��    ��:����ͬ��ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void SyncMealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= sync_column1st_number+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}

 /*******************************************************************************
 * ��������:AbnomalMealNameDisp                                                                  
 * ��    ��:�����¼ʱ��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 							
void AbnomalMealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 15; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= record_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,meat_name,10);break;	
			case 0x02:mystrcat(temp,chicken_name,8);break;		
			case 0x03:mystrcat(temp,duck_name,8);break;	
			case 0x04:mystrcat(temp,fish_name,8);break;
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}

/*******************************************************************************
 * ��������:AbnomalMealCnttDisp                                                                 
 * ��    ��:�����¼ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void AbnomalMealCnttDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= record_column1st_cnt_t+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}

/*******************************************************************************
 * ��������:AbnomalMealCntDisp                                                                 
 * ��    ��:�����¼ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void AbnomalMealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= record_column1st_cnt+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}

 /*******************************************************************************
 * ��������:DisplayUserRecord                                                                     
 * ��    ��:��ʾ����ʱ�Ĵ����¼                                                              
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/  
void DisplayUserRecord(void)
{
	uint8_t cnt_t=0,floor=0;
	for(floor=0;floor<4;floor++)		//�����Ļ����
	{
		AbnomalMealNameDisp(0,floor);
		AbnomalMealCnttDisp(0xff,floor);
		AbnomalMealCntDisp(0xff,floor);		
	}
	floor=0; //��ԭ
	if(UserAct.MealCnt_1st>0)
	{
		AbnomalMealNameDisp(cnt_t+1,floor);
		AbnomalMealCnttDisp(UserAct.MealCnt_1st_t,floor);
		AbnomalMealCntDisp(UserAct.MealCnt_1st,floor);		
		floor++;
	}
	cnt_t++;
	if(UserAct.MealCnt_2nd>0)
	{
		AbnomalMealNameDisp(cnt_t+1,floor);
		AbnomalMealCnttDisp(UserAct.MealCnt_2nd_t,floor);
		AbnomalMealCntDisp(UserAct.MealCnt_2nd,floor);	
		floor++;
	}
	cnt_t++;
	if(UserAct.MealCnt_3rd>0)
	{
		AbnomalMealNameDisp(cnt_t+1,floor);
		AbnomalMealCnttDisp(UserAct.MealCnt_3rd_t,floor);
		AbnomalMealCntDisp(UserAct.MealCnt_3rd,floor);	
		floor++;
	}
	cnt_t++;
	if(UserAct.MealCnt_4th>0)
	{
		AbnomalMealNameDisp(cnt_t+1,floor);
		AbnomalMealCnttDisp(UserAct.MealCnt_4th_t,floor);
		AbnomalMealCntDisp(UserAct.MealCnt_4th,floor);	
		floor++;
	}
	if(UserAct.MoneyBack>0)
	{
		//��ʾ�û��Ѹ� ��  Ӧ��
		VariableChage(record_UserActPayAlready,UserAct.PayAlready);
		//Ӧ�˵�Ǯ = �Ѹ���Ǯ - ����Ʒ������*�۸�
		UserAct.MoneyBack = UserAct.PayAlready -((UserAct.MealCnt_1st_t-UserAct.MealCnt_1st)* price_1st+ (UserAct.MealCnt_2nd_t-UserAct.MealCnt_2nd)* price_2nd+ (UserAct.MealCnt_3rd_t-UserAct.MealCnt_3rd)* price_3rd+(UserAct.MealCnt_4th_t-UserAct.MealCnt_4th)* price_4th);
		VariableChage(record_UserActPayBack,UserAct.MoneyBack);
	}
	else
	{
		VariableChage(record_UserActPayAlready,0);
		VariableChage(record_UserActPayBack,0);
	}
}
	
 /*******************************************************************************
 * ��������:GetPassWord                                                                     
 * ��    ��:ԭʼ���� ���Լ����޸����뺯��                                                               
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                    
 *******************************************************************************/  
void GetPassWord(unsigned char *PassWord)
{
  PassWord[0] = 5;
	PassWord[1] = 1;
	PassWord[2] = 8;
	PassWord[3] = 5;
	PassWord[4] = 1;
	PassWord[5] = 8;
}

void GetAdminPassWord(unsigned char *PassWord)
{
  PassWord[0] = 1;
	PassWord[1] = 2;
	PassWord[2] = 3;
	PassWord[3] = 4;
	PassWord[4] = 5;
	PassWord[5] = 6;
}
 /*******************************************************************************
 * ��������:VerifyPassword                                                                   
 * ��    ��:����Ƚ�                                                                 
 *                                                                               
 * ��    ��:����ָ��1������ָ��2����Ƚϵĳ���                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                     
 *******************************************************************************/  
uint8_t VerifyPassword( uint8_t *Src1, uint8_t *Src2, uint8_t Length)
{
  uint8_t i = 0; 
	for(i = 0; i < Length; i++)
	{
	  if(*(Src1+i) != *(Src2+i))
		{
		    return 0;
		}
	}
	return 1;
}
 /*******************************************************************************
 * ��������:ChangeVariableValues                                                                   
 * ��    ��:�ı����ݱ�����ֵ                                                                                                                          
 * ��    ��:������ַ,��������,��������                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 
char meat_cnt_t= 0,chicken_cnt_t= 0,duck_cnt_t= 0,fish_cnt_t= 0; /*��ʱ����*/
uint8_t  PassWordLen=0;	//����ĳ���Ϊ0
uint8_t  PassWord[6]={0};
uint8_t  InputPassWord[6]={0};
bool cardbalence_cancel_flag= false;
bool mealneed_sync = false;  //��Ʒͬ�����
void ChangeVariableValues(int16_t VariableAdress,char *VariableData,char length)
{
	char MealID =0;
	uint16_t coins_time=0;
	uint8_t i= 0,j=0;
	  switch (VariableAdress)
		{
			case meal_choose: /*�������û�ѡ��*/
			{
        CloseTIM4();
				switch(VariableData[1])
				{
					case 0x01:
						if(DefineMeal[0].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserAct.MealID= VariableData[1]; //��ǰ�û�ѡ�͵�ID
							UserAct.MealCnt_1st_t= 1;//����Ĭ�Ϸ���Ϊ 1							
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();
							PageChange(Meal1st_interface);//��ʾ��Ӧ����
              VariableChage(meat_cnt,0x01);
 							MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x02:
						if(DefineMeal[1].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_2nd_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();							
							PageChange(Meal2rd_interface);//��ʾ��Ӧ����	
							VariableChage(chicken_cnt,0x01);
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;							
					case 0x03:
						if(DefineMeal[2].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_3rd_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();							
							PageChange(Meal3ns_interface);//��ʾ��Ӧ����	
							VariableChage(duck_cnt,0x01);
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x04:
					{					
						if(DefineMeal[3].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_4th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);							
							OpenTIM3();	
						  VariableChage(count_dowm,WaitTime);
							PageChange(Meal4th_interface);//��ʾ��Ӧ����	
							VariableChage(fish_cnt,0x01);
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;
					case 0x05:  /*�����û���*/
					{
						PageChange(Password_interface);
					}break;
					default:break;
				}
			}break;      				
			case meat_cnt: /*���ܲ��������*/
			{
				if(VariableData[1]<= DefineMeal[0].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_1st_t= VariableData[1];
					UserAct.MealCost_1st = GetMealPrice(UserAct.MealID,UserAct.MealCnt_1st_t);//��һЩ�ظ�����
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st_t);
				}
				else
				{
					UserAct.MealCnt_1st_t= DefineMeal[0].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st_t);
					VariableChage(meat_cnt,UserAct.MealCnt_1st_t);	//�ı������ַ����
				}	 						 
			}break;
			case chicken_cnt:/*�㹽��������*/
			{
				if(VariableData[1]<= DefineMeal[1].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_2nd_t= VariableData[1];
					UserAct.MealCost_2nd = GetMealPrice(UserAct.MealID,UserAct.MealCnt_2nd_t);//��һЩ�ظ�����
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd_t);
				}
				else
				{
					UserAct.MealCnt_2nd_t= DefineMeal[1].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd_t);
					VariableChage(chicken_cnt,UserAct.MealCnt_2nd_t);	//�ı������ַ����
				}	 						 
			}break;
			case duck_cnt:/*��Ƥ��Ѽ*/
			{
				if(VariableData[1]<= DefineMeal[2].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_3rd_t= VariableData[1];
					UserAct.MealCost_3rd = GetMealPrice(UserAct.MealID,UserAct.MealCnt_3rd_t);//��һЩ�ظ�����
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd_t);
				}
				else
				{
					UserAct.MealCnt_3rd_t= DefineMeal[2].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd_t);
					VariableChage(duck_cnt,UserAct.MealCnt_3rd_t);	//�ı������ַ����			
				}	 
			}break;	
			case fish_cnt:/*�������*/
			{
				if(VariableData[1]<= DefineMeal[3].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_4th_t= VariableData[1];
					UserAct.MealCost_4th = GetMealPrice(UserAct.MealID,UserAct.MealCnt_4th_t);//��һЩ�ظ�����
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th_t);
				}
				else
				{
					UserAct.MealCnt_4th_t= DefineMeal[3].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th_t);
					VariableChage(duck_cost,UserAct.MealCnt_4th_t);	//�ı������ַ����
				}
			}break;	
   
      case mealcnt_choose:/*��ҳѡ��ť*/
      {
			  switch(VariableData[1])
				{
					case 0x01:  /*���빺�ﳵ*/
					{
								
					}break;
					case 0x02:  /*����ѡ��*/
					{
            PutIntoShopCart();
						PageChange(Menu_interface);
						WaitTimeInit(&WaitTime);
					}break;
 					case 0x03:  /*���빺�ﳵ*/
					{
            PutIntoShopCart();
						if(UserAct.Meal_totoal>0)
						{
							SettleAccounts();
							CloseTIM3();
							WaitTimeInit(&WaitTime);
							OpenTIM7();
							Current= waitfor_money;//�����Ǯ����
							PlayMusic(VOICE_2);
						}
					}break;
					case 0x04:  /*ȡ��*/
					{
						//������е��û�����???
						ClearUserBuffer();
						PageChange(Menu_interface);
						CloseTIM3();
						Current= current_temperature;//ȡ��������г���
						
					}break;
					default:break;
				}
			}break;
			/*�ı������в�Ʒ������*/
			/*��Ҫ��Ʒ��ID���к�:column1st_count, MealID*/
			case column1st_count:MealID= DispMeal[0].meal_id;goto loop1;
			case column2nd_count:MealID= DispMeal[1].meal_id;goto loop1;
			case column3rd_count:MealID= DispMeal[2].meal_id;goto loop1;
			case column4th_count:MealID= DispMeal[3].meal_id;goto loop1;
			{
loop1:	switch(MealID)
				{
					case 0x00:
					{
						VariableChage(VariableAdress,0); //ά��ԭ����ѡ������
					}break;
					case 0x01:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//���û�ѡ������С�ڻ���ڴ��ʱ
				    {
					    UserAct.MealCnt_1st =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_1st =UserAct.MealCnt_1st *price_1st; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_1st); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_1st); //�ı䵥�ܼۣ���ַƫ��1
				    }
				    else
				    {	
							VariableChage(VariableAdress,UserAct.MealCnt_1st); //ά��ԭ����ѡ������
				    }					
					}break;
					case 0x02:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_2nd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_2nd =UserAct.MealCnt_2nd *price_2nd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_2nd); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_2nd); //�ı䵥�ܼ�
				    }
				    else
				    {
					    VariableChage(VariableAdress,UserAct.MealCnt_2nd); //ά��ԭ����ѡ������
				    }									
					}break;
					case 0x03:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_3rd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_3rd =UserAct.MealCnt_3rd *price_3rd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_3rd); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_3rd); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserAct.MealCnt_3rd); //ά��ԭ����ѡ������
				    }							
					}break;
					case 0x04:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_4th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_4th =UserAct.MealCnt_4th *price_4th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_4th); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_4th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserAct.MealCnt_4th); //ά��ԭ����ѡ������
				    }							
					}break;
					default:break;						
				}
				UserAct.PayShould= (UserAct.MealCost_1st+UserAct.MealCost_2nd+UserAct.MealCost_3rd+UserAct.MealCost_4th);
	      VariableChage(mealtotoal_cost,UserAct.PayShould);
			}break;
			case payment_method: /*���ʽ*/
			{
				AcountCopy();
				if(UserAct.PayShould==0) goto loop7;
				PageChange(Acount_interface+2); //�����¸������ת����һ��ҳ���ֹ�����Ӽ�
				switch(VariableData[1])
				{
					case 0x01:   /*�ֽ�֧��*/
					{
						CurrentPoint =2;
						PlayMusic(VOICE_3);
						if(!OpenCashSystem()){};// printf("cash system is erro2");  //�ر��ֽ����
					}break;
					case 0x02:   /*����Ԥ����*/
					{
						CurrentPoint =7;
						PlayMusic(VOICE_4);
						if(!CloseCashSystem()){};// printf("cash system is erro3");  //�ر��ֽ����
					}break;
					case 0x03:   /*����֧ͨ��*/
					{
						CurrentPoint =8;
						PlayMusic(VOICE_4);
						if(!CloseCashSystem()){};//printf("cash system is erro4");  //�ر��ֽ����
						
					}break;
					case 0x04:   /*ȡ��*/
					{
loop7:			if(!CloseCashSystem()){};//printf("cash system is erro5");  //�ر��ֽ����
						CloseTIM3();
						CloseTIM7();
						CurrentPoint = 0 ;
						UserAct.MoneyBack= UserAct.PayAlready; //��ʱ���յ���Ǯ��Ӳ�ҵ���ʽ����
            ClearUserBuffer();
						UserAct.Cancle= 0x01;
						PageChange(Menu_interface);
						Current= hpper_out;
					}break;
					default:break;		
				}					
			}break;
			case caedbalence_cancel:/*ˢ��ȡ��*/
			{
				PageChange(Acount_interface+2);
				cardbalence_cancel_flag=true;
			}break;			
			case bill_print:
			{
				switch(VariableData[1])
				{
					case 0x01:   /*��ӡСƱ*/
					case 0x02:   /*����ӡСƱ*/
					{
						UserAct.PrintTick= VariableData[1];
							 /*�ж��Ƿ��ӡСƱ*/ 			
            PrintTickFun(&UserAct.PrintTick);
            CloseTIM4();
            if(!erro_flag) //���д����ʱ�򲻽�����ͽ���						
						PageChange(Mealout_interface);
					}break;  
					default:break;
				}
			}break;
			case password:
			{
				switch(VariableData[1])
				{
					case 0x00:
					case 0x01:
					case 0x02:
					case 0x03:
					case 0x04:
					case 0x05:
					case 0x06:
					case 0x07:
					case 0x08:
					case 0x09:
					case 0x0A:						
					case 0x0B:
					case 0x0C:
					case 0x0D:
					case 0x0E:
					case 0x0F:
					{
	          if(PassWordLen < 6)
		        {
		          InputPassWord[PassWordLen] = (VariableData[1]);
		          PassWordLen++;
			        DisplayPassWord(PassWordLen);//��ʾһ���ַ���******
		        }
	        }break;					
					case 0x10:/*�˸��*/
					{
	          if(PassWordLen > 0)
		        {
	            InputPassWord[PassWordLen] = 0;
	            PassWordLen--;
		          DisplayPassWord(PassWordLen);
		        }
					}break;
					case 0x11:/*ȷ�ϼ�*/ //��Ҫ���볬������
					{
	          GetPassWord(PassWord);
    	      if(VerifyPassword(PassWord, InputPassWord,6) == 1) //�Ų�������ȷ
		        {
							PassWordLen = 0;		
							DisplayPassWord(0);//���������ʾ
						  if(erro_flag!=0) 
						  {
							  break;
						  }							
							//����д�������break;
		           /*�����Ʒ���ý���*/
		           PageChange(MealSet_interface);					 
							 InitSetting();//��յ�һ�����е����� //�ԷŲ͵����ݽ��г�ʼ��
			         PassWordLen = 0;break;
		        }
						GetAdminPassWord(PassWord);
    	      if(VerifyPassword(PassWord, InputPassWord,6) == 1) //����Ա�˻�
		        {
							//�������Ա�������ã���Ҫ�����ض��Ĺ���ѡ��
		           /*�������Ա����*/
							 if(erro_flag!=0) 
							 {
								 PageChange(UserAbonamalRecord_interface);
								 DisplayUserRecord();
							   DisplayPassWord(0);//���������ʾ
			           PassWordLen = 0;							 			 
								 break;
							 }
							 else
							 {
								 PageChange(Coinset_interface);
								 VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);
								 DisplayPassWord(0);//���������ʾ
								 PassWordLen = 0;break;
							 }
		        }
		        else
		        {
		          /*������֤���󣬽��������������*/
		           PageChange(Password_interface);
							 DisplayPassWord(0);							
			         PassWordLen = 0;
		        }						
	        }break;						
					case 0x12:/*����*/
					{
						if(erro_flag!=0) 
						{
							 break;
						}								
		        PageChange(Menu_interface);
						DisplayPassWord(0);//���������ʾ
		        PassWordLen = 0;	
					}break;
          default:break;					
        } 					
			}break;
			case record_clear:
			{
				switch(VariableData[1])
			  {
					case 0x01: //�������
					{
						erro_flag=0; //������ݱ��
						//Current = hpper_out;
						//UserAct.Cancle = 0x01; //�൱��ȡ������
						
   				}break;
					case 0x02: //����
					{
						PageChange(Err_interface);
					}break;
          default:break;
				}	
			}break;
			case admin_set:
			{
				switch(VariableData[1])
				{ 
					case 0x01:  /*�¶����ð�ť*/
					{
						PageChange(TemperatureSet_interface);
					}break;
					case 0x02:  /*��Ʒ���ð�ť Ӧ�ø�Ϊһ������������� */
					{
						//VariableChage(meal_num,0x01);
						//PageChange(MealInput_interface);
							for(i=0;i<90;i++)
	            {
	              FloorMealMessageWriteToFlash.FlashBuffer[i] = 0 ;
	            }
	            WriteMeal();  //д���Ʒ����
							StatisticsTotal(); 
							DispLeftMeal();//
					}break;
					case 0x03:	/*ȡ����*/
					{
						if(mealneed_sync == false)
						{
						  PageChange(Menu_interface);
						}
					}
          default:break;
				}					
			}break;
			case meal_num:
			{
				uint8_t cnt_t=0;
			  CurFloor.MealID= VariableData[1];	
				InitSetting();
				for(cnt_t = 0; cnt_t < FloorMealNum; cnt_t++)  //���Ҳ�
				{
					if(FloorMealMessageWriteToFlash.FloorMeal[cnt_t].MealCount ==0)
					{
						 CurFloor.FloorNum = cnt_t+1;
						 break;
					}			
				}
				VariableChage(floor_num,CurFloor.FloorNum);				
			}break;
      case floor_num:
			{
			  CurFloor.FloorNum= VariableData[1];
				InitSetting();
			}break;
      case row_1st:
      {
				CurFloor.FCount= VariableData[1];
			}break;	
      case row_2nd:
      {
				CurFloor.SCount= VariableData[1];
			}break;	
      case row_3rd:
      {
				CurFloor.TCount= VariableData[1];
			}break;	
      case mealinput_choose:
			{
				switch(VariableData[1])
				{
					case 0x01:  /*����*/
					{
						CurFloor.FCount = 3;
		        CurFloor.SCount = 3;
		        CurFloor.TCount = 3; 
		        CurFloor.MealCount = 9;
					  VariableChage(row_1st,CurFloor.FCount);
		        VariableChage(row_2nd,CurFloor.SCount);
		        VariableChage(row_3rd,CurFloor.TCount);	
					}break;
					case 0x02:  /*���*/
					{
						CurFloor.FCount = 0;
		        CurFloor.SCount = 0;
		        CurFloor.TCount = 0; 
		        CurFloor.MealCount = 0;
					  VariableChage(row_1st,CurFloor.FCount);
		        VariableChage(row_2nd,CurFloor.SCount);
		        VariableChage(row_3rd,CurFloor.TCount);							
					}break;
					case 0x03:  /*ȷ��*/
					{
						/*�����ܹ��Ĳ�Ʒ��*/
						CurFloor.MealCount=  CurFloor.FCount + CurFloor.SCount + CurFloor.TCount;
						/*�ѵ�ǰ��һ�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FCount    = CurFloor.FCount;
						/*�ѵ�ǰ��2�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].SCount    = CurFloor.SCount;
						/*�ѵ�ǰ��3�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].TCount    = CurFloor.TCount;
						/*�ѵ�ǰ���ò͵�ID�ű�������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealID    = CurFloor.MealID;
						/*�ѵ�ǰ���õĲ͵�������������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealCount = CurFloor.MealCount;
						/*�����ò͵Ĳ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FloorNum  = CurFloor.FloorNum;
						/*���ֲ�Ʒ*/
						for(j = 0; j < 4; j++)	 //4�����ID��
						{
							/*�����͵���Ŀ����*/
							DefineMeal[j].MealCount = 0;
							/*�����͵��ж�Ӧ����Ŀ����*/
							for(i = 0; i < FloorMealNum; i++)
							{
								DefineMeal[j].Position[i][0] = 0;
								DefineMeal[j].Position[i][1] = 0;
								DefineMeal[j].Position[i][2] = 0;
							}
							 /*ͳ�Ƹ����͵���Ŀ���ܺ�*/
							for(i = 0; i < FloorMealNum; i++)
							{														 /*�͵�ID�Ǵ�1-4*/
								if(FloorMealMessageWriteToFlash.FloorMeal[i].MealID == j + 1)
								{
									DefineMeal[j].MealCount      = DefineMeal[j].MealCount + FloorMealMessageWriteToFlash.FloorMeal[i].MealCount;
									DefineMeal[j].Position[i][0] = FloorMealMessageWriteToFlash.FloorMeal[i].FCount;
									DefineMeal[j].Position[i][1] = FloorMealMessageWriteToFlash.FloorMeal[i].SCount;
									DefineMeal[j].Position[i][2] = FloorMealMessageWriteToFlash.FloorMeal[i].TCount;
									//printf("DefineMeal[%d].Position[%d][0]=%d\r\n",j,i,DefineMeal[j].Position[i][0]);
								}
							}
						}
						StatisticsTotal(); //��ͳ����Ŀ��д��
						WriteMeal(); //д���ݵ�flash
						DispLeftMeal(); //���²�Ʒ����
						mealneed_sync = true;
						//printf("DefineMeal[%d].MealCount=%d\r\n",i,DefineMeal[i].MealCount);
					}break;
					case 0x04:  /*����ͬ��*/
					{
						uint32_t mealcompare_data=0;
						uint8_t mealsample_data=0,cnt_t=0,floor=0;
						for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
						{
							/*��ʾ��Ʒ,����*/									
              SyncMealNameDisp(0,cnt_t);
              SyncMealCntDisp(0xFF,cnt_t); 									 
						}	
						//��ֹ��Ļ���*/
            ScreenControl(ScreenDisable);
						/*��Ʒͬ��ʱ����������������Ͽ������*/
						/*���ܼ���ǩ��������ֱ������*/
						//if(!SignInFunction())  AbnormalHandle(signin_erro);
						/*�ϴ�����������ݣ�����̨ʵʱ��������*/
            SendtoServce(); 
						//����ͬ���ӳ���
						MealDataCompareFun();
						if(MealCompareData.MealCompareTotoal==0xFFFFFFFF) //��ȷ
						 {
							 mealneed_sync = false;
							 PageChange(Data_synchronization);
							 for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
							 {
									 /*��ʾ��Ʒ,����*/									
                  SyncMealNameDisp(cnt_t+1,floor);
                  SyncMealCntDisp(DefineMeal[cnt_t].MealCount,floor); 
                  floor++;									 
							 }							 
						 }
						 /*���Լ���Աȵķ���ֵ�����ж�*/
             else
             {
							 mealneed_sync = true;
							 PageChange(Data_synchronization+2);
							 for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
							 {
								 if(MealCompareData.MealComparePart[cnt_t]==0xff)
								 {
									//��Ʒͬ���ɹ������κ����� 
								 }
								 else
								 {
									 /*��ʾ��Ʒ,����*/									
                  SyncMealNameDisp(cnt_t+1,floor);
                  SyncMealCntDisp(MealCompareData.MealComparePart[cnt_t],floor); 
                  floor++;									 
								 }
               }						 
						 }
						//��ʱʱ�˳��������˳�
						ScreenControl(ScreenEnable);
						DispLeftMeal();//�ı�ʣ���Ʒ������ʾ
					}break;
					case 0x05:  /*����*/
					{
						CurFloor.FCount    = 0;
						CurFloor.SCount    = 0;
						CurFloor.TCount    = 0;
						CurFloor.MealID    = 0;
						CurFloor.MealCount = 0;
						CurFloor.FloorNum  = 0;	 
	          PageChange(MealSet_interface);
					}break;
					default:break;	
				}
			}break;
			case temprature_set:
			{
				SetTemper(VariableData[1]);
			//VariableData[1]; �¶ȱ���=VariableData[1];
			}break;
			case coins_key:  //��һ����һ��
      {
				uint16_t cnt_t=0;
				if(VariableData[1]==0x01)
				{
					Coins_cnt=0;
					coins_time= (CoinsTotoalMessageWriteToFlash.CoinTotoal/10);
					VariableChage(coins_back,Coins_cnt);
					//�����ܷ��ͼ����Ǽ����ķ����ǿ��еģ�//CoinsTotoalMessageWriteToFlash.CoinTotoal = SendOutN_Coin(CoinsTotoalMessageWriteToFlash.CoinTotoal);
					cnt_t = SendOutN_Coin(CoinsTotoalMessageWriteToFlash.CoinTotoal);
					VariableChage(coins_in,cnt_t);//��ʾ����Ӳ����
					VariableChage(coins_back,CoinsTotoalMessageWriteToFlash.CoinTotoal-cnt_t);					
//					  do
//						{//��������
//							VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ����
//							VariableChage(coins_back,Coins_cnt);
//							delay_ms(10);
//							if((CoinsTotoalMessageWriteToFlash.CoinTotoal-cnt_t)==Coins_cnt)break;
//						}while(1);				
				}
				else if(VariableData[1] == 0x02)
				{
					StopRefond();
					PageChange(Menu_interface);
				}
			}break;	
      case 	coins_in:
      {	
				CoinsTotoalMessageWriteToFlash.CoinsCnt[0]= VariableData[1];
				CoinsTotoalMessageWriteToFlash.CoinsCnt[1]= VariableData[0];
				WriteCoins();
				VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ����
				
			}break;			
			  default:break;		
		}
}
 /*******************************************************************************
 * ��������:DealSeriAceptData                                                                   
 * ��    ��:����ӵ�����Ļ���ص�����                                                        
 *          ���ʵ���������������ݣ�0xA5 0XA5 0X5A                                                                           
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 	
void DealSeriAceptData(void)
 {
	unsigned char i;
	unsigned char temp,temp1,length,checkout;
	char RegisterAdress=0;
	int16_t VariableAdress=0;
	unsigned char Rx3DataBuff[10]={0};/*����һ�������С��?����Խ��(out of bounds),?�ɱ䳤������*/
	char RegisterData[5]={0};  //�Ĵ�����������
	char VariableData[5]={0};  //������������
	char VariableLength= 0;   //�������ݵĳ���
	while(UART3_GetCharsInRxBuf()>=9) //��ȡ��������С��ֱ��������������
	{	
		if(USART3_GetChar(&temp)==1)
		{	
			if(temp==FH0)	//ƥ��֡ͷ��һ���ֽ�
			{ 
loop:	  if(USART3_GetChar(&temp1)==1)  
				{
					if(temp1==FH1)  //ƥ��֡ͷ�ڶ����ֽڣ�
					{
						if(USART3_GetChar(&length)==1)  //��ȡ�����ֽڳ���
						{
							checkout =1;//��λ��ʼ��ȡ���������ݱ�־	
						}	
					}
					else if(temp1==FH0)	goto loop; //�����������0xA5������ж���һ���ֽ��Ƿ�Ϊ0x5A
				}
			}
		}
	  if(checkout==1) //ֱ����ȡ���ݳ��ȣ�Ȼ�������
	  {
	 	  checkout =0;  //��λ��־
		  for(i=0;i<length;i++) USART3_GetChar(Rx3DataBuff+i);
		  if(Rx3DataBuff[0]==0x81)  //���Ĵ�����������
		  {
			  RegisterAdress =Rx3DataBuff[1];
			  for(i=0;i<(length-2);i++)
			  RegisterData[i]=Rx3DataBuff[2+i];
			  //�����޸�������ݵĹ���
		  }
		  else if(Rx3DataBuff[0]==0x83) //�����ݴ洢����������
		  {
			  myunion.adr[1] =Rx3DataBuff[1]; //�п�����С��ģʽ�����
			  myunion.adr[0] =Rx3DataBuff[2];
			  VariableAdress =myunion.adress;
			  VariableLength =Rx3DataBuff[3];
			  for(i=0;i<(length-4);i++)
			  VariableData[i]=Rx3DataBuff[4+i];
			  //�����޸ı�����ַ���ݵĹ���
			  ChangeVariableValues(VariableAdress,VariableData,VariableLength);
		   }
	   } 
	 }		
}

