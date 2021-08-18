#ifndef __UART1_H__
#define __UART1_H__

extern unsigned char Uart1_Receive_Data;
extern unsigned char Uart1_Receive_Change;
extern unsigned char Uart1_Busy;

void Uart1_Init(void);//功能描述: 传统51单片机串口初始化
void UART1_SendStr(char *s);//功能描述: 串口单字节发送程序
void Uart1_SendOneByte(unsigned char Uart_Sent_Data);//功能描述: 串口发送字符串

#endif