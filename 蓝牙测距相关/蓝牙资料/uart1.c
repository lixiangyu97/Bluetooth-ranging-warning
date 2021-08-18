
/*
	STC89C52RC  UART驱动模块
	
	注: 默认外部晶振 11.0592MHz 9600bps
	
	每次接收数据Uart1_Receive_Change置位，Uart1_Receive_Data储存接收到的数据
	
	使用串口需要占用定时器1

*/

#include <reg52.h>
#include "uart1.h"


unsigned char Uart1_Receive_Data;
unsigned char Uart1_Receive_Change=0;
unsigned char Uart1_Busy=0;

/*
  功能描述: 串口单字节发送程序
  函数参数: Uart_Sent_Data-发送的数据
  返回说明: 无 
*/
void Uart1_SendOneByte(unsigned char Uart_Sent_Data)
{
	while(Uart1_Busy);//等待上次串口发射数据完毕
	Uart1_Busy=1;
	SBUF = Uart_Sent_Data;
}

/*
  功能描述: 传统51单片机串口初始化
  函数参数: 无
  返回说明: 无 
	注：默认设定为9600pbs，数据位8，停止位1，无校验，使用串口需要占用定时器1
*/
void Uart1_Init(void)
{
    TMOD |= 0x20;
    SCON |= 0x50;
    TH1 = 0xFD;	//根据晶振和波特率要求改变数值
    TL1 = TH1;
    PCON |= 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}

/*
  功能描述: 串口发送字符串
  函数参数: s —— 指向字符串的指针(字符串以'\0'结尾)
  返回说明: 无 
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
  功能描述: 串口中断服务程序
  函数参数: 无
  返回说明: 无
	注意：每次接收到数据，Uart1_Receive_Change置位，接受的数据储存在Uart1_Receive_Data数据中。
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