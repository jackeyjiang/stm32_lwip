#include "stm32f4xx_usart.h"
#include "stdio.h"
#define LINE_LENGTH 20  /*�л�������С������ÿ�����������ַ���*/
/*��׼�ն��豸�У�����ACSII�붨�壬�����޸�*/
#define In_BACKSP  0x08		       /*ASCII <--(�˸��)*/
#define In_DELETE  0x7F			   /*ASCII <DEL> (DEL��)*/
#define In_EOL  '\r'			   /*ASCII <CR> (�س���)*/
#define In_SKIP '\3'			   /*ASCII control-C*/
#define In_EOF  '\x1A'			   /*ASCII control-Z*/
#define Out_DELETE  "\x8 \x8"	   /*VT100 backspace and clear*/
#define Out_SKIP  "^C\n"		   /*C and new line*/
#define Out_EOF  "^Z" 			   /*^Z and return EOF*/

/****************************************************
*��    ��:putchar()
*��    ��:���׼�ն��豸����һ���ֽ�����
*��ڲ���:ch:�����͵��ַ�
*���ں���:�������ַ�
˵     ��:printf��������øú�����Ϊ�ײ����������Ӵ�������ַ���PC��
          �����նˣ�printf�Ľ������ӡ�������ն���
****************************************************/
//int putchar(int ch)
int fputc(int ch, FILE *f)
{
  if(ch=='\n')  //'\n'
  {
    USART_SendData(USART3,0x0d);    //'\r'
  }
  USART_SendData(USART3,ch);   //�Ӵ��ڷ�������
  return(ch);     //�����͵�����ԭ�����ر����ɹ�
}
/****************************************************
*��    ��:put_message()
*��    ��:���׼�ն��豸����һ���ַ���
*��ڲ���:��
*���ں���:��
*˵    ��:��
****************************************************/
static void put_message(char *s)
{
  while(*s)			 //��ǰ�ַ���Ϊ��(�ַ�����0x00��β)
  {
	 putchar(*s++);  //���һ���ַ���ָ��ָ����һ���ַ�
  }
}

/****************************************************
*��    ��:getchar()
*��    ��:���׼�ն��豸����һ���ֽ�����
*��ڲ���:ch:�����͵��ַ�
*���ں���:�������ַ�
*˵    ��:printf��������øú�����Ϊ�ײ����������Ӵ�������ַ���PC��
          �����նˣ�printf�Ľ������ӡ�������ն���
****************************************************/
//int getchar(void)
//{
//  char ch;
//  ch= USART_ReceiveData(USART3);
//  putchar(ch);
//  return(ch);
//}

//int getchar(void)
int fgetc(FILE *f)
{
  static char io_buffer[LINE_LENGTH+2];   /*Where to put chars*/
  static int ptr;                         /*point to buffer*/
  char c;
  while(1)
  {
    if(io_buffer[ptr])
	  return (io_buffer[ptr++]);
	ptr= 0;
	while(1)
	{
	  c= USART_ReceiveData(USART3); 
	  if(c== In_EOF&&!ptr)			     //---EOF��(Ctrl+Z)---
	  {									 //EOF��ֻ����δ���������ַ�ʱ����Ч
	    put_message(Out_EOF);			 //���ն���ʾEOF��
		return EOF;
	  }
	  if((c==In_DELETE)||(c==In_BACKSP))  //---�˸��DEL��---
	  {
	    if(ptr)
		{
		  ptr--;						  //��������ɾ��һ���ַ�
		  put_message(Out_DELETE);		  //���ն���ʾҲɾ��һ���ַ�
		}
	  }
	  else if(c==In_SKIP)				  //---ȡ����Ctrl+C---
	  {
	    put_message(Out_SKIP);			  //���ն���ʾ�������е�����һ��
		ptr =LINE_LENGTH+1;				  /*������Զ��0(������)*/
		break;
	  }
	  else if(c==In_EOL)				  //---�����س���---
	  {
	    putchar(io_buffer[ptr++]='\n');	  //���ն˻���
		io_buffer[ptr]=	0;				  //ĩβ����ַ������ַ�NULL
		ptr =0;							  //ָ�����
		break;							  //������ʼ��������
	  }
	  else if(c<LINE_LENGTH)			  //---����ACSII���ַ�---
	  {
	    if(c>=' ')						  //ɾ��0x20���µ�����ACSII��
		{
		  putchar(io_buffer[ptr++]=c);	  //���뻺����
		}
	  }
	  else
	  {									  //---����������---
	    putchar('\7'); //���ն˷��������0x07,PC����һ������ʾ����
	  }
	}
  }
}





