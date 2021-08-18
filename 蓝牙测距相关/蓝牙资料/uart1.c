
/*
	STC89C52RC  UART����ģ��
	
	ע: Ĭ���ⲿ���� 11.0592MHz 9600bps
	
	ÿ�ν�������Uart1_Receive_Change��λ��Uart1_Receive_Data������յ�������
	
	ʹ�ô�����Ҫռ�ö�ʱ��1

*/

#include <reg52.h>
#include "uart1.h"


unsigned char Uart1_Receive_Data;
unsigned char Uart1_Receive_Change=0;
unsigned char Uart1_Busy=0;

/*
  ��������: ���ڵ��ֽڷ��ͳ���
  ��������: Uart_Sent_Data-���͵�����
  ����˵��: �� 
*/
void Uart1_SendOneByte(unsigned char Uart_Sent_Data)
{
	while(Uart1_Busy);//�ȴ��ϴδ��ڷ����������
	Uart1_Busy=1;
	SBUF = Uart_Sent_Data;
}

/*
  ��������: ��ͳ51��Ƭ�����ڳ�ʼ��
  ��������: ��
  ����˵��: �� 
	ע��Ĭ���趨Ϊ9600pbs������λ8��ֹͣλ1����У�飬ʹ�ô�����Ҫռ�ö�ʱ��1
*/
void Uart1_Init(void)
{
    TMOD |= 0x20;
    SCON |= 0x50;
    TH1 = 0xFD;	//���ݾ���Ͳ�����Ҫ��ı���ֵ
    TL1 = TH1;
    PCON |= 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}

/*
  ��������: ���ڷ����ַ���
  ��������: s ���� ָ���ַ�����ָ��(�ַ�����'\0'��β)
  ����˵��: �� 
*/
void UART1_SendStr(char *s)
{
	while( *s != '\0')
	{
		Uart1_SendOneByte( *s );	
		s ++;
	}
}

/*
  ��������: �����жϷ������
  ��������: ��
  ����˵��: ��
	ע�⣺ÿ�ν��յ����ݣ�Uart1_Receive_Change��λ�����ܵ����ݴ�����Uart1_Receive_Data�����С�
*/
void UARTInterrupt(void) interrupt 4
{
	if(RI)
  {
		RI = 0;
		Uart1_Receive_Data=SBUF;
		Uart1_Receive_Change=1;
  }
  if(TI)
	{
		TI = 0;
		Uart1_Busy=0;
	}
    
}