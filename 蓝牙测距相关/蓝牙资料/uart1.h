#ifndef __UART1_H__
#define __UART1_H__

extern unsigned char Uart1_Receive_Data;
extern unsigned char Uart1_Receive_Change;
extern unsigned char Uart1_Busy;

void Uart1_Init(void);//��������: ��ͳ51��Ƭ�����ڳ�ʼ��
void UART1_SendStr(char *s);//��������: ���ڵ��ֽڷ��ͳ���
void Uart1_SendOneByte(unsigned char Uart_Sent_Data);//��������: ���ڷ����ַ���

#endif