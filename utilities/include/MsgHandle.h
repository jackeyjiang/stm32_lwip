#ifndef __MSGHANDLE__
#define __MSGHANDLE__
#include "stdint.h"
#include "stm32f4xx.h"
#define FloorMealNum  15				  /*定义有多少个行*/
typedef struct MealAt
{
    
	uint8_t MealPrice;         //餐价格
	uint8_t Position[15][3];   //餐放置的位置
	uint8_t MealCount;         //餐剩余总数
	
}MealAttribute;	  ////定义4个菜系的属性。

extern MealAttribute DefineMeal[4];	//定义4个菜系

typedef struct UserAction
{
  uint8_t MealCnt_1st_t;      //用户选择第一份餐品的数量未按下放入购物车键时的临时数量
	uint8_t MealCnt_2nd_t;      //用户选择第二份餐品的数量未按下放入购物车键时的临时数量
	uint8_t MealCnt_3rd_t;      //用户选择第三份餐品的数量未按下放入购物车键时的临时数量
	uint8_t MealCnt_4th_t;      //用户选择第四份餐品的数量未按下放入购物车键时的临时数量	
  uint8_t MealCnt_1st;      //用户选择第一份餐品的数量
	uint8_t MealCnt_2nd;      //用户选择第二份餐品的数量
	uint8_t MealCnt_3rd;      //用户选择第三份餐品的数量
	uint8_t MealCnt_4th;      //用户选择第四份餐品的数量
  uint16_t MealCost_1st;    //用户选择第一份餐品的总价
	uint16_t MealCost_2nd;    //用户选择第二份餐品的总价
	uint16_t MealCost_3rd;    //用户选择第三份餐品的总价
	uint16_t MealCost_4th;    //用户选择第四份餐品的总价
	uint16_t PayShould;        //用户应付款总额
	uint8_t  MealID;           //用户当前选择的餐品ID
	uint8_t  Meal_totoal;      //用户选餐的总数
	uint8_t  Meal_takeout;     //用户已取出的餐品数
	uint16_t PayForCoins;           //用户投入的硬币数	
	uint16_t PayForBills;           //用户投入的纸币数
	uint16_t PayForCards;           //用户应经刷卡的数
	uint16_t PayAlready;            //用户已经付款总额
	int16_t  MoneyBack;              //用户找零数  //有符号性避免数量减为0xfffe
	uint8_t  PrintTick;              //是否打印小票标志
	uint8_t  PayType ;              //支付方式
	uint8_t  Cancle;                //用户取消购买
}UserSelection;

extern UserSelection UserAct;


typedef struct FloorMeal
{
  uint8_t MealID;	     //当前层存放餐的ID
	uint8_t MealCount;	 //当前层存放餐的总数
	uint8_t FCount;      //第一列放餐的数量
	uint8_t SCount;	     //第二列放餐的数量
	uint8_t TCount;	     //第三列放餐的数量
	uint8_t FloorNum;	   //当前是第几层
}FloorAttribute;

		  

typedef union _FloorMealMessage
{
  FloorAttribute      FloorMeal[FloorMealNum];
  uint8_t	            FlashBuffer[FloorMealNum*6]; /*这里我改变了一个*/
}FloorMealMessage;
extern 	FloorMealMessage FloorMealMessageWriteToFlash;/*将数据写入flash的结构*/
extern  FloorAttribute FloorMeal[FloorMealNum], CurFloor;
extern uint8_t WaitTime ;
extern int MoneyBack ;

typedef union _CoinsTotoalMessage
{
  uint16_t CoinTotoal;
	uint8_t  CoinsCnt[2];
}CoinsTotoalMessage;
extern CoinsTotoalMessage CoinsTotoalMessageWriteToFlash;/*硬币数结构体*/ 

typedef union _MealCompareData
{
	uint32_t MealCompareTotoal;
	uint8_t  MealComparePart[4];
}MealCompareDataStruct;
extern MealCompareDataStruct MealCompareData; /*餐品对比数据显示*/


void StatisticsTotal(void);
void MealArr(unsigned char index);
void InitSetting(void);

#endif
