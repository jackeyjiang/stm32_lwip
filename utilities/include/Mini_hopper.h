#ifndef __Mini_hopper_h
#define __Mini_hopper_h

extern uint32_t Coins_cnt; //һ���˱Ҽ���
extern uint32_t Coins_totoal; //Ӳ������ͳ��
extern uint8_t ErrorType;
void InitMiniGPIO(void);
u8 SendOutN_Coin(int num);
uint8_t StatisticalCoin(void);
void CloseCoins(void);
void OpenCoins(void);
void RefundButton(void);
void StopRefond(void);

#endif

