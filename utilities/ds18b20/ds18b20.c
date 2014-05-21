#include "ds18b20.h"
#include <stdio.h>
#include "delay.h"
#include "printf.h"
GPIO_InitTypeDef  GPIO_InitStructure;

//CRC = X8 + X5 + X4 + 1
const u8 Crc8Table [256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53}; 

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure a SysTick Base time to 1 us.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 1 usec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000000)) //SysTick���ú���
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick�ж����ȼ�����
}

/*******************************************************************************
* Function Name  : DS18B20_Set_Input
* Description    : ����DS18B20��IO��DQ Ϊ����.
* Input          : None
* Output         : None
* Return         : void
*******************************************************************************/
void DS18B20_Set_Input(void)
{
  /* Configure the GPIO pin */
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : DS18B20_Set_Output
* Description    : ����DS18B20��IO��DQ Ϊ���.
* Input          : None
* Output         : None
* Return         : void
*******************************************************************************/
	 
void DS18B20_Set_Output(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(DS18B20_GPIO_CLK, ENABLE);
  /* Configure the GPIO pin */
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : DS18B20_Init
* Description    : ��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���.
* Input          : None
* Output         : None
* Return         : 0 : �ɹ�  1 ��ʧ��
*******************************************************************************/	 
u8 DS18B20_Init(void)
{

 
  /* Enable the Clock */
  RCC_AHB1PeriphClockCmd(DS18B20_GPIO_CLK, ENABLE);

  /* Configure the GPIO pin */
  DS18B20_Set_Output();

  DS18B20_Reset();
  return DS18B20_Check();
}  
/*******************************************************************************
* Function Name  : DS18B20_Reset
* Description    : //��λDS18B20  ��λ���壨���Ϊ480uS�ĵ͵�ƽ�źţ�.
* Input          : None
* Output         : None
* Return         : 0 : �ɹ�  1 ��ʧ��
*******************************************************************************/
void DS18B20_Reset(void)	   
{        
	DS18B20_Set_Output(); //SET AS OUTPUT
	DS18B20_OUT_LOW;      //����DQ
    delay_us(650);        //����650us
    DS18B20_OUT_HIGH;
	delay_us(10);       // 10US
}



/*******************************************************************************
* Function Name  : DS18B20_Reset
* Description    : �ȴ�DS18B20�Ļ�Ӧ.
* Input          : None
* Output         : None
* Return         : 0 :����  1 ��δ��⵽DS18B20�Ĵ���
*******************************************************************************/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
    
	DS18B20_Set_Input();    // �����������ͷŴ��߲�������շ�ʽ��Rx����
	// DSl820 �ȴ�15.60uS ���ҽ��ŷ��ʹ������壨60-240uS�ĵ͵�ƽ�źš���
    while ((DS18B20_DATA_IN == 1) && (retry<60))
	{
		retry++;
		delay_us(2);
	};	 
    
	if(retry>=60)
        return 1;
    
	retry=0;
    while ((DS18B20_DATA_IN == 0) && (retry<245))
	{
		retry++;
		delay_us(2);
	};
	if(retry>=245)
        return 1;    
    
	return 0;
}


/*******************************************************************************
* Function Name  : DS18B20_Reset
* Description    : �ȴ�DS18B20�Ļ�Ӧ.
* Input          : None
* Output         : None
* Return         : 0 :����  1 ��δ��⵽DS18B20�Ĵ���
*******************************************************************************/
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
    
	DS18B20_Set_Output();   //SET AS OUTPUT
    DS18B20_OUT_LOW; 
	delay_us(3);
    DS18B20_OUT_HIGH; 
	DS18B20_Set_Input();    //SET AS INPUT
	delay_us(6);
	if(DS18B20_DATA_IN)
    {
        data=1;
    }
    else
    {
        data=0;  
    }
    delay_us(50); 
    
    return data;
}
/*******************************************************************************
* Function Name  : DS18B20_Read_Byte
* Description    : ��DS18B20��ȡһ���ֽ�.
* Input          : None
* Output         : None
* Return         : ����ֵ������������
*******************************************************************************/
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=dat>>1;
        dat+=(j<<7);
    }	

    return dat;
}
/*******************************************************************************
* Function Name  : DS18B20_Write_Byte
* Description    : дһ���ֽڵ�DS18B20.
* Input          : dat��Ҫд����ֽ�
* Output         : None
* Return         : void
*******************************************************************************/
void DS18B20_Write_Byte(u8 dat)     
{             
    u8 j;
    u8 testb;
	DS18B20_Set_Output();//SET AS OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;

        DS18B20_OUT_LOW;// Write 1
        delay_us(2);  
        if (testb) 
        {
            DS18B20_OUT_HIGH;
        }
        delay_us(55);
        DS18B20_OUT_HIGH;
        delay_us(5);                          
    }
}
/*******************************************************************************
* Function Name  : DS18B20_Get_Temp
* Description    : /��ds18b20�õ��¶�ֵ
* Input          : None
* Output         : None
* Return         : //����ֵ���¶�ֵ ��-550~1250��
*******************************************************************************/
short DS18B20_Get_Temp(void)
{
    u8 i, temp;
    u8 TL,TH;
	short tem;
    u8 str[9];
    
    //��ʼ�¶�ת��
    DS18B20_Reset();	   
	DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert

    //��ʼ��ȡ�¶�
    DS18B20_Reset();
	DS18B20_Check();
 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	
    //TL=DS18B20_Read_Byte(); // LSB   
    //TH=DS18B20_Read_Byte(); // MSB  

    for (i=0;i<9;i++) 
	{    
	    str[i] = DS18B20_Read_Byte();
    }
    if(GetCRC(str, 9) == 0)
      //  printf(" CRC OK  ");
		;
    else
     //   printf(" CRC ERR ");
	 ;

    for (i=0;i<9;i++) 
	{    
	   // printf(" %02X", str[i]);
    }  
    
    TL = str[0]; // LSB   
    TH = str[1]; // MSB      
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }
    else
    {
        temp=1;//�¶�Ϊ��         
    }
    tem=TH<<8 | TL; //��ò�������λ��11λ�¶�ֵ

    //ת�� *0.625
    tem = tem*10;
    tem = tem>>4;

	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
} 

/************************************************************
*Function:CRCУ��
*parameter:
*Return:
*Modify:
*************************************************************/
u8 GetCRC(u8 *str, u32 length)
{ 
    u8 crc_data=0;
   	u32 i;
  	for(i=0;i<length;i++)  //���У��
    { 
    	crc_data = Crc8Table[crc_data^str[i]];
    }
    return (crc_data);
}

u8 DS18B20_Get_Serial(u8 *serial)
{
    u8 i;
    u8 crcdata;    

    DS18B20_Reset();
	DS18B20_Check();

    DS18B20_Write_Byte(0x33);// read rom

    for(i=0;i<8;i++)
    {
       *(serial + i) = DS18B20_Read_Byte();
    }
    
    crcdata = GetCRC(serial,8); // Ϊ0��У����ȷ

    return (crcdata);
}
/*******************************************************************************
* Function Name  : DS18B20_DEMO
* Description    : DS18B20_DEMO����.  ���ô˺������Եõ�DS18B20�ɼ����¶�
* Input          : void
* Output         : None
* Return         : void
*******************************************************************************/ 	 
 short Temperature;
 uint8_t DS18B20_DEMO(void)
 {	
  	 

	{	 
        if(DS18B20_Init())// ���18B20  ���ﲻ�Ǳ���
        {
 //           printf("\r\n ����18B20�¶ȴ����Ƿ�װ��...");

			return 1 ;

        }
        
		Temperature=DS18B20_Get_Temp();
		if(Temperature<0)
		{
			Temperature=-Temperature;
			printf("-");
		}						 
//	    	printf("\r\n DEMO  ��ǰ�¶�Ϊ:%d.%dC\n\r",Temperature/10, Temperature%10);
		   Temperature = Temperature/10;

		 return   0 ;
	}
}

