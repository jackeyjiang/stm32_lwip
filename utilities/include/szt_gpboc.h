#ifndef __szt_gpboc__h
#define __szt_gpboc__h


#define DataHead 0x00
#define BeginCode1 0x00
#define BeginCode2 0xff

#define EndCode 0x00


#define Year_Frist 0x20

typedef enum {
T_QueryStatus = 0x20, //״̬��ѯ
T_UserLogin = 0x40,	  //����Ա��¼
T_TimeCheck = 0x24,	  //ʱ��ͬ��
T_SztDeductOnce = 0x42,	 //���η�����ͨ�ۿ�
T_SztDeductAgain = 0x82, //�ٴη�����ͨ�ۿ�
T_SztReadCardOnce = 0x68,  //���ζ�����ͨ��
T_SztReadCardAgain = 0x98, //�ٴζ�����ͨ��
T_SztUnlink = 0x22,		 //����ͨ�Ͽ�����
T_SztCheck_In = 0x78,	  //����ͨ����ǩ�����������ء������ϴ�
T_SztRechargeOnce = 0x54,	 //��������ͨ��ֵ����
T_SztRechargeAgain = 0x94,	 //�ٴ�����ͨ��ֵ����
T_GpbocCheck_In = 0xC2,		 //����ǩ��
T_GpbocQueryReadCard = 0xC6,  //������ѯ���
T_GpbocDeduct = 0xC8,		  //�����ۿ�
T_GpbocSendData = 0xCA,		 //�������ͼ�¼
T_GpbocCheck_Out = 0xCC,	 //�������ᡢǩ��
T_M1Mutual =0x7A,		   //M1�������
T_Sleep = 0xAC,			   //����
T_Stop = 0xAE			   //�ػ�
 
}SendOrderStatus;

typedef enum {
R_QueryStatus = 0x21,	  //״̬��ѯ
R_UserLogin = 0x41,		  //����Ա��¼
R_TimeCheck = 0x25,		  //ʱ��ͬ��
R_SztDeductOnce = 0x43,	   //���η�����ͨ�ۿ�
R_SztDeductAgain = 0x83,   //�ٴη�����ͨ�ۿ�
R_SztReadCardOnce = 0x69,  //���ζ�����ͨ��
R_SztReadCardAgain = 0x99, //�ٴζ�����ͨ��
R_SztUnlink = 0x23,		  //����ͨ�Ͽ�����
R_SztCheck_In = 0x79,	   //����ͨ����ǩ�����������ء������ϴ�
R_SztRechargeOnce = 0x55,  //��������ͨ��ֵ����
R_SztRechargeAgain = 0x95, //�ٴ�����ͨ��ֵ����
R_GpbocCheck_In = 0xC3,		//����ǩ��
R_GpbocReadCard = 0xC7,  //������ѯ���
R_GpbocDeduct = 0xC9,		 //�����ۿ�
R_GpbocSendData = 0xCB,		//�������ͼ�¼
R_GpbocCheck_Out = 0xCD,   //�������ᡢǩ��
R_M1Mutual =0x7B		     //M1�������
}RecOrderStatus;

typedef enum{
RxNo = 0,
RxOk = !RxNo
}RxStatus;

typedef enum{
NormalFlag = 0,
AckFlag =1,
DataFlag = 2
}Usart2DataStatus;

typedef enum{
IsNc = 0,
IsAck =1,
IsNack =2,
IsAckError =3
}AckReturnStatus;//Ack ����״̬

typedef enum{
DataNc = 0,	  //�޷�������
DataReOk =1,  //���ϸ�ʽ������
DataReErr =2   //�������ݲ����ϸ�ʽ������
}DataReStatus;// ��������״̬


typedef struct {
uint8_t datahead;		//����ͷ
uint8_t startPacket[2]; //��ʼ��
uint8_t dataLen[2];		//��saddr��ʼ��Dcs֮ǰ���е����ݳ���
uint8_t saddr[5];		//һ��̶�Ϊ06,FF,FF,FF,FF 
uint8_t taddr[5];		//һ����05��ͷ������������sam���ϵĿ��ţ��Ƚ�ʮ����ת����ʮ�����ƣ��ӵ�λ��ʼ��ȡ
uint8_t startNum;		//��������ֵ��һ��Ϊ0
uint8_t SendCommand;	//���͵�����
uint8_t plen1[2];		//�ܹ����ٰ����ݣ�һ��Ϊ1������00��01
uint8_t plen2[2];		//�����,һ��Ϊ00��01
uint8_t Dcs;			//У���룬��ֵ���ϴ�saddr��ʼ����ֵ֮ǰ������ֵ�����Ϊ0
uint8_t dataend;		 //����β
}sendorderDef;


typedef struct{
uint8_t datahead;		  //����ͷ
uint8_t startParcket[2];  //��ʼ��
uint8_t dataLen[2];		  //��saddr��ʼ��Dcs֮ǰ���е����ݳ���
uint8_t saddr[5];		  //�ն˶�����Sam���ţ���05��ͷ������Sam����
uint8_t taddr[5];		  //��λ�������Sam���ţ���06��ͷ������Sam����
uint8_t startNum;		  //��ʼ��������ֵ����ӵ��������е�ֵ��ͬ��һ��Ϊ00
uint8_t RevCommand;		  //��������ʲô����ص�ֵ
uint8_t plen1[2];		  //�ܹ����ٰ����ݣ�һ��Ϊ1������00��01
uint8_t plen2[2];		  //�����,һ��Ϊ00��01
uint8_t resultNum;		  //�������
uint8_t Dcs;			  //У���룬��ֵ���ϴ�saddr��ʼ����ֵ֮ǰ������ֵ�����Ϊ0
uint8_t dataend;		  //����β
}revDataDef;

typedef struct {
	uint8_t Device_ID[4];				//�豸ID   4 BYTE BIN
	uint8_t Terminal_ID;			//����ͨ�ն˱��  4 BYTE BIN
	uint8_t SoftWare_Ver[10];					//�豸����汾��  10 BYTE ASCII
	uint8_t Device_Status[4];			//�豸״̬��   4 BYTE BIN
	int32_t Curr_RemainMoney;		//��ǰʣ����   4 BYTE BIN
	uint8_t Curr_ParFileName[28];				//��ǰ��Ȳ�����  28 BYTE ASCII
	uint16_t Curr_OKRecNum;			//��ǰδ�ϴ����׼�¼��  2 BYTE BIN
	uint16_t Curr_BadRecNum;		//��ǰδ�ϴ��Ҽ�¼��  2 BYTE BIN
	uint8_t Curr_DeviceTime[8];		//��ǰ�豸ʱ����Ϣ  8 BYTE BCD
}DeviceInf;	   //״̬��ѯ��������������Ϣ



typedef struct{
uint8_t Year_H;
uint8_t Year_L;
uint8_t Month;
uint8_t Day;
uint8_t Hours;
uint8_t Minutes;
uint8_t Senconds;
uint8_t WeekDay;
}MachineTimeDef;   //�豸����ͬ��ʱ��ֵ


typedef struct{
uint8_t CardStatus;   //��״̬��������Ϊ0x00
uint32_t CardNum;     //����
uint8_t CardType;	  //������
int32_t SztMoney;	   //����ǰ���
int32_t MachinMoney;   //�豸��ǰʣ����
int32_t SztDeposit;	   //��Ѻ��
uint8_t SellCardDay[4];	  //����������
uint8_t GetRentsMonth;	  //�����·�
int32_t RentsNum;		  //Ӧ�����
int32_t ReturnableMoney;  //����ʵ�ʽ��
int32_t Poundage;		  //�˿�������
int32_t NoSellCardDeposit;	//δ�ۿ�Ѻ��
int32_t NoSellCardMoney;	//δ�ۿ����
}SztReadCardInfStr;   //����ͨ������������ֵ



typedef struct{
int32_t BeginMoney;   //����ͨ�ۿ�ǰ���
int32_t EndMoney;     //����ͨ�ۿ����
uint32_t CardNum;     //����ͨ����
uint32_t TradeNum;    //���׺�
uint32_t TACNum;       //TAC��
int32_t MachineAmount;   //�豸��Ȩ���
uint32_t machineNum;    //�豸�ն˺�
}SztReductInfStr;


typedef struct  {
        uint8_t   ucLogInLogVersion; //���ṹ��İ汾�ţ������Ժ���в�ͬ�汾����ͬ����
        uint8_t   ucAcqBankNum; 	   //��ǩ����Ӧ���յ��кţ���1���� **
        uint8_t   ucLogInOK;         //���յ���ǩ���ɹ���
        uint8_t   ucOper;            //����Ա��
	    uint8_t   ucKeyMode;         //ǩ��������Կ���ͣ�0x30, ����˫������0x31,��������
        uint32_t   lBatchNumber;      /* ���κ�*/
        uint8_t   szPosId[9];        /*  POS �ն˺�         ANS8    */
        uint8_t   szMerchantId[16];   //�洢15�ַ������̻��� ANS15
} GpbocInitInf;	 //������ʼ��ǩ����������������Ϣ

typedef struct {
	uint8_t SztStatus;  //����ͨ��״̬��0x00��ʾ���д�������ͨ���ݣ����8���ֽ�ȫΪ0
	uint32_t SztNum;  //����ͨ����
	int32_t SztMoney;  //����ͨ�����
	uint8_t SztReserve[64];  //����ͨ������
	uint8_t GpbocStatus;  //������״̬��0x00��ʾ���д����������ݣ����24���ֽ�ȫΪ0
	uint8_t GpbocNum[20];  //��������
	int32_t GpbocMoney;  //���������
	uint8_t GpbocReserve[64];	//����������
}GpbocReadCardInf;//��������ʱ�����ص��������ݣ�������m1������


typedef struct  __attribute__ ((__packed__)) {
        uint8_t   iIccDataLen;            //��������������˳����VA���׼�¼�����෴
        uint8_t   sIccData[255];          //55������
        uint8_t   bPanSeqNoOk;            // ADVT case 43 [3/31/2006 Tommy]
        uint8_t   ucPanSeqNo;             // App. PAN sequence number
        uint8_t   sAppCrypto[8];          // app. cryptogram, 9F26, already in F55
        uint8_t   sAuthRspCode[2];        //"Y1"
        uint8_t   sTVR[5];
        uint8_t   szAID[32+1];
        uint8_t   szAppLable[16+1];
        uint8_t   sTSI[2];
        uint8_t   sATC[2];
        uint8_t   sAIP[2];
        uint8_t   sCVR[4];
        uint8_t   szAppPreferName[16+1];
        uint8_t   szCardNoFull[21];         //��VAPOSLOG�¼ӣ�δ��*����ǰ6��4֮���λ�õ�ȫ����
		uint8_t   szMerchantId[16];       //2013-04-23�¼ӣ�������λ����¼����������Ϣ���洢15�ַ������̻��� ANS15
} CRYPT_LOG_STRC;


//�滮����192�ֽ�(48��˫�ֽڿ�)RSA��Կ�����潻�׼�¼����ܲ���CRYPT_LOG_STRC���м��ܡ�ÿ��ʵ�ʿ���PAYLOAD���ر���Կ������1
#define LOG_RSAKey_BYTES        192
#define LOG_RSAKey_DWORDS       (192/4)
#define LOG_RSA_BLOCKLOAD_BYTES (LOG_RSAKey_BYTES-1)
#define LOGPUR_CRYPTDATA_BLOCKS ((sizeof(CRYPT_LOG_STRC)+LOG_RSA_BLOCKLOAD_BYTES-1)/LOG_RSA_BLOCKLOAD_BYTES)
#define LOGPUR_CRYPTDATA_BYTES  (LOGPUR_CRYPTDATA_BLOCKS * LOG_RSAKey_BYTES)

typedef struct  __attribute__ ((__packed__))   {
       // uint16_t  TransLogLen;      //���ṹ���������ݵĳ��ȣ�������
        uint8_t   ucLogVersion;      //���ṹ��İ汾�ţ������Ժ���в�ͬ�汾����ͬ���� **
        uint8_t   ucAcqBankNum;      //�ý��׶�Ӧ���յ��кţ���1���� 
        uint32_t  iTransNo;       //���׺�,ʵΪ�������ͣ�VA��EC�̶�Ϊ66
        uint8_t   ucBlklistFail;    //��������
        uint8_t   ucqPBOCSaleFail;  //�ǽӽ���ʧ��
        uint8_t   ucqPBOCLastRd;    //�����һ����¼ʧ��
        uint8_t   ucOfflineSaleFail;  //IC�ѻ��ܾ�

       // uint8_t   sAmount[6];   //��VAPOSLOG���٣�������lTransAmount��������
        uint32_t  lTransAmount;   //��VAPOSLOG�¼ӣ���������λ�����潻�׽��
        uint32_t  lCardBalance;   //��VAPOSLOG�¼ӣ���������λ�����濨EC���
        uint32_t  lTraceNo;       // ��ˮ��
        uint32_t  lBatchNumber;   // ���κ�

        uint8_t   szPosId[9];     //  POS �ն˺�          N(8)    
        uint8_t   szCardNo[21];   //���ݸ���λ��ʱ����Ҫ��*����ǰ6��4֮���λ��
        uint8_t   szDate[9];
        uint8_t   szTime[7];
        uint8_t   szExpDate[5];
        uint8_t   szTellerNo[4];  //����Ա
        uint8_t   szCardUnit[4];  //����֯��CUP VIS MAS

        uint32_t  BkDataLen;      //���ݽ������ݵĳ���
        //����Ϊ���ܺ��ֱ��õĽ������ݣ���λ��ʹ�� ����λ��ʹ�ýṹ��StrucBkData
        
//		union {
//             CRYPT_LOG_STRC  StrucBkData;  //__attribute__ ((__packed__)) 
//             uint8_t   CryptBkData[LOGPUR_CRYPTDATA_BYTES];
//        } ;
		
		/*CRYPT_LOG_STRC  StrucBkData; //�������ݰ�*/
} TRANSLOG_TO_HOST;   //QPBOC���Ѽ�¼

/*
SLIM_LOG_STRC qpbocslimlog;
Memcpy(&qpbocslimlog, DeductOutData + 4 , sizeof(TRANSLOG_TO_HOST));
*/


typedef struct  __attribute__ ((__packed__))   {
       uint8_t   ucUpLogVersion;    //���ṹ��İ汾�ţ������Ժ���в�ͬ�汾����ͬ����
       uint8_t   ucAcqBankNum; 	   //��ǩ����Ӧ���յ��кţ���1���� **
       uint8_t   ucUploadOK;        //���յ���ǩ���ɹ���
       uint32_t  lBatchNumber;      //���κ�
       uint32_t  lTotalNum;         //��Ҫ�����ļ��е��ܱ���
       uint32_t  lToUploadNum;      //������Ҫ���͵��ܱ�������Щʧ�ܽ��׵Ȳ������ͣ�
       uint32_t  lSuccNum;          //�ɹ����͵ı���
       uint32_t  lSuccTotalAmt;     //�ɹ����͵��ܽ��
       uint32_t  lFailNum;          //����ʧ�ܵı���
       uint32_t  lFailTotalAmt;     //����ʧ�ܵ��ܽ��
       uint8_t   szPosId[9];        //POS �ն˺�  ANS8   
       uint8_t   szMerchantId[16];   //�洢15�ַ������̻��� ANS15
} UPLOADLOG_TO_HOS;	 //�����ϴ����ݷ������ݽ�����



typedef struct  __attribute__ ((__packed__))   {
      uint8_t   ucLogInLogVersion; //���ṹ��İ汾�ţ������Ժ���в�ͬ�汾����ͬ����
      uint8_t   ucAcqBankNum; 	   //��ǩ����Ӧ���յ��кţ���1���� **
      uint8_t   ucLogInOK;         //���յ���ǩ���ɹ���
      uint8_t   ucOper;            //����Ա��
	  uint8_t   ucKeyMode;         //ǩ��������Կ���ͣ�0x30, ����˫������0x31,��������
      uint32_t  lBatchNumber;      /* ���κ�*/
      uint8_t   szPosId[9];        /*  POS �ն˺�         ANS8    */
      uint8_t   szMerchantId[16];   //�洢15�ַ������̻��� ANS15
} LOGINLOG_TO_HOST;  //�������ᡢǩ�˷������ݽ�����


void Uart2_Configuration(void);
void Open_Szt_Gpboc(void);
void Close_Szt_Gpboc(void);
void return_ACK(void);
void return_NACK(void);
void SztFixData(uint8_t LenH,uint8_t LenL,uint8_t OrderNum);
void GpbocFixData(uint8_t LenH,uint8_t LenL,uint8_t OrderNum);
uint8_t ManageACK(void);
uint8_t ManageRxData(void);
uint8_t ManageReOrderType(void);

void Order_Ifn_QueryStatus(void);
void Order_SztUserLogin(void);
void Order_SztTimeCheck(void);
void Order_SztDeductOnce(int32_t money);
void Order_SztDeductAgain(int32_t money);
void Order_SztReadCardOnce(void);
void Order_SztReadCardAgain(void);
void Order_SztUnlink(void);
void Order_SztAutoCheckIn(void);
void Order_SztAutoDownload(void);
void Order_SztAutoSendData(void);
void Order_SztRechargeOnce(void);
void Order_SztRechargeAgain(void);

void Order_Gpboc_Init(void);
void Order_Gpboc_ReadCard (void);
void Order_Gpboc_Deduct(uint32_t submoney);
void Order_Gpboc_SendData(void);
void Order_Gpboc_Check_Out(void);

uint8_t ManageQueryStatus(void);
uint8_t ManageSztUserLogin(void);
uint8_t ManageSztTimeCheck(void);
uint8_t ManageSztUnlink(void);
uint8_t ManageSztAutoManage(void);
uint8_t ManageSztReadCardOnce(void);
uint8_t ManageSztReadCardAgain(void);
uint8_t ManageSztDeductOnce(void);
uint8_t ManageSztDeductAgain(void);
uint8_t ManageSztRechargeOnce(void);
uint8_t ManageSztRechargeAgain(void);

uint8_t ManageGpbocInit(void);
uint8_t ManageGpbocDeduct(void);
uint8_t ManageGpbocReadCard(void);
uint8_t ManageGpbocSendData(void);
uint8_t ManageGpbocCheckOut(void);

void ManageGpbocAll(uint8_t PutInNum);

uint8_t Gpboc_CheckIn (void);
uint8_t OnlyReadGpbocCard(void);
uint8_t GpbocDeduct(uint32_t money_deduct);
uint8_t UpperGpboc(void);
uint8_t GpbocVout(void);

void TestSzt(uint8_t PutInNum);

uint8_t Szt_CheckIn(void);
uint8_t SztDeduct(int32_t money);
uint8_t SztAutoSend(void);

bool Szt_GpbocAutoCheckIn(void);

uint8_t  BCDtoDec(uint8_t bcd);
uint8_t DectoBCD(uint8_t Dec); 







#endif
