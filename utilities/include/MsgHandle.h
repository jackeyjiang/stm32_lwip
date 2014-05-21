#ifndef __MSGHANDLE__
#define __MSGHANDLE__
#include "stdint.h"
#include "stm32f4xx.h"
#define FloorMealNum  15				  /*�����ж��ٸ���*/
typedef struct MealAt
{
    
	uint8_t MealPrice;         //�ͼ۸�
	uint8_t Position[15][3];   //�ͷ��õ�λ��
	uint8_t MealCount;         //��ʣ������
	
}MealAttribute;	  ////����4����ϵ�����ԡ�

extern MealAttribute DefineMeal[4];	//����4����ϵ

typedef struct UserAction
{
  uint8_t MealCnt_1st_t;      //�û�ѡ���һ�ݲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint8_t MealCnt_2nd_t;      //�û�ѡ��ڶ��ݲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint8_t MealCnt_3rd_t;      //�û�ѡ������ݲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint8_t MealCnt_4th_t;      //�û�ѡ����ķݲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����	
  uint8_t MealCnt_1st;      //�û�ѡ���һ�ݲ�Ʒ������
	uint8_t MealCnt_2nd;      //�û�ѡ��ڶ��ݲ�Ʒ������
	uint8_t MealCnt_3rd;      //�û�ѡ������ݲ�Ʒ������
	uint8_t MealCnt_4th;      //�û�ѡ����ķݲ�Ʒ������
  uint16_t MealCost_1st;    //�û�ѡ���һ�ݲ�Ʒ���ܼ�
	uint16_t MealCost_2nd;    //�û�ѡ��ڶ��ݲ�Ʒ���ܼ�
	uint16_t MealCost_3rd;    //�û�ѡ������ݲ�Ʒ���ܼ�
	uint16_t MealCost_4th;    //�û�ѡ����ķݲ�Ʒ���ܼ�
	uint16_t PayShould;        //�û�Ӧ�����ܶ�
	uint8_t  MealID;           //�û���ǰѡ��Ĳ�ƷID
	uint8_t  Meal_totoal;      //�û�ѡ�͵�����
	uint8_t  Meal_takeout;     //�û���ȡ���Ĳ�Ʒ��
	uint16_t PayForCoins;           //�û�Ͷ���Ӳ����	
	uint16_t PayForBills;           //�û�Ͷ���ֽ����
	uint16_t PayForCards;           //�û�Ӧ��ˢ������
	uint16_t PayAlready;            //�û��Ѿ������ܶ�
	int16_t  MoneyBack;              //�û�������  //�з����Ա���������Ϊ0xfffe
	uint8_t  PrintTick;              //�Ƿ��ӡСƱ��־
	uint8_t  PayType ;              //֧����ʽ
	uint8_t  Cancle;                //�û�ȡ������
}UserSelection;

extern UserSelection UserAct;


typedef struct FloorMeal
{
  uint8_t MealID;	     //��ǰ���Ų͵�ID
	uint8_t MealCount;	 //��ǰ���Ų͵�����
	uint8_t FCount;      //��һ�зŲ͵�����
	uint8_t SCount;	     //�ڶ��зŲ͵�����
	uint8_t TCount;	     //�����зŲ͵�����
	uint8_t FloorNum;	   //��ǰ�ǵڼ���
}FloorAttribute;

		  

typedef union _FloorMealMessage
{
  FloorAttribute      FloorMeal[FloorMealNum];
  uint8_t	            FlashBuffer[FloorMealNum*6]; /*�����Ҹı���һ��*/
}FloorMealMessage;
extern 	FloorMealMessage FloorMealMessageWriteToFlash;/*������д��flash�Ľṹ*/
extern  FloorAttribute FloorMeal[FloorMealNum], CurFloor;
extern uint8_t WaitTime ;
extern int MoneyBack ;

typedef union _CoinsTotoalMessage
{
  uint16_t CoinTotoal;
	uint8_t  CoinsCnt[2];
}CoinsTotoalMessage;
extern CoinsTotoalMessage CoinsTotoalMessageWriteToFlash;/*Ӳ�����ṹ��*/ 

typedef union _MealCompareData
{
	uint32_t MealCompareTotoal;
	uint8_t  MealComparePart[4];
}MealCompareDataStruct;
extern MealCompareDataStruct MealCompareData; /*��Ʒ�Ա�������ʾ*/


void StatisticsTotal(void);
void MealArr(unsigned char index);
void InitSetting(void);

#endif
