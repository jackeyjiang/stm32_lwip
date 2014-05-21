#ifndef __PROTOCOL_H
#define __PROTOCOL_H


typedef struct
{
  uint8_t reack;  //ack��־,0��ʾ��ack,1��ʾ��ack
  uint8_t renack;	//nack��־��0��ʾ��nack,1��ʾ��nack
  uint8_t redoor;	//��״̬��־��0����1��
  uint8_t rerelative;	//�������λ�ã����ԭ�㣩��־��0��ʾ���ڴ���λ�ã�1��ʾ�ڴ���λ��
  uint8_t regoal;	  //����ȡ�͵㣬�ȴ�ȡ�������־��0��ʾ����ȡ�͵㣬1��ʾ�ѵ�ȡ�͵㣬�ȴ�ȡ������
  uint8_t retodoor;	   //������Ϳڱ�־��0��ʾδ������Ϳڣ�1��ʾ������Ϳڣ��ȴ�ȡ�߲�
  uint8_t remealaway;	//���ѱ�ȡ�߱�־��0��ʾ��δ��ȡ�ߣ�1��ʾ���ѱ�ȡ��
  uint8_t reenablegetmeal;	 //ȡ��5�뻹ȡ�����ͱ�־��0��ʾȡ��������1��ʾ����ȡ����
  uint8_t remealnoaway; 	//���ͿڵĲͳ���20�뻹δ��ȡ�߱�־��0��ʾ��̬����ѱ�ȡ�ߣ�1��ʾ���ͿڵĲͳ���20�뻹δ��ȡ��
}Urart6RecFlagInf;

typedef struct
{
bool E101;
bool E102;
bool E103;
bool E201;
bool E301;
bool E401;
bool E501;
bool E502;
bool E503;
bool E601;
bool E711;
bool E070;
bool E000;
bool E010;
bool E020;
bool E030;
bool E032;
bool E040;
bool E050;
}ErrorFlagInf;


extern uint8_t Usart6Buff[6];	//���ڽ��ջ�е�ַ�����������
extern uint8_t Usart6Index;
extern uint8_t Usart6DataFlag;   //�˱�־��Ϊ�Ƿ������ݽ��գ�0�ޣ�1�����ݣ�ƽʱ��0
extern Urart6RecFlagInf machinerec;
extern uint8_t RetryFre;  //�ط�ԭ����������������
extern uint8_t LinkTime;
extern uint8_t Line ,Column  ;
extern ErrorFlagInf ErFlag;
extern uint8_t TemperSign ;

void Uart6_Configuration(void);

void TIM5_Init(void);
void OpenTIM5(void);
void CloseTIM5(void);

void SendAck(void);
void SendNack(void);
void MachineInit(void);
void SendCoord(uint8_t floor,uint8_t row);
void SendLink(void);
void StopSell(void);
void GetMeal(void);
void SetTemper(uint8_t temper);

void manageusart6data(void);

uint8_t OrderSendLink(void);
uint8_t OrderMachineInit(void);
uint8_t OrderSendCoord(uint8_t floor,uint8_t row);
uint8_t OrderStopSell(void);
uint8_t OrderGetMeal(void);
uint8_t OrderSetTemper(uint8_t inputtemper);
void OnlymachieInit(void);


#endif


