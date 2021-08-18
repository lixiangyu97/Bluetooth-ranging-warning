#include <reg52.h>
#include "uart1.h"
#include "Timer0.h"

sbit Speaker_Pin=P2^0;
sbit LED_Pin=P0^0;


unsigned char MS100_Con=0;
unsigned char OneSec_Con=0;

void delay3s(void)   //延迟3秒
{
    unsigned char a,b,c;
    for(c=85;c>0;c--)
        for(b=188;b>0;b--)
            for(a=85;a>0;a--);
}

void main()
{
	LED_Pin=0;
	delay3s();
	LED_Pin=1;
	Uart1_Init();//串口初始化
	Timer0_Init();//定时器初始化
	while(1)
	{
		if(MS100_Con>=10)
		{
			MS100_Con=0;
			Uart1_SendOneByte(0xa0);//串口发送0xa0
		}
		if(Uart1_Receive_Change)
		{
			Uart1_Receive_Change=0;
			if(Uart1_Receive_Data==0xa0)//检测接收到的数据是否是a0
			{
				OneSec_Con=0;
				Speaker_Pin=1;
				LED_Pin=~LED_Pin;//控制LED闪烁
			}
		}
		if(OneSec_Con>=100)
		{
			OneSec_Con=0;
			Speaker_Pin=0;
			LED_Pin=0;
		}
	}
}

/*
  功能描述: 定时器0中断函数
  函数参数: 无
  返回说明: 无
*/

void Timer0_Interrupt(void) interrupt 1	//	定时器0中断函数
{
	
	TH0 = 0xDc;	//重置定时时间，如果初始化使用的是定时方式2则不需要重置
	TL0 = 0x00;
  MS100_Con++;
	OneSec_Con++;
}