#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include  <math.h>    //Keil library  
#include "delay.h"

sbit buzzer=P2^0;

unsigned long times_20ms=0;		  	//定时20ms计时
unsigned char baojignFlag = 0;//报警标志
unsigned char reportFlag = 0;//定时上报处理

void Init_Timer0(void);//函数声明
void UART_Init(void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{
	Init_Timer0();        //定时器0初始化
	UART_Init();

	DelayMs(100);          //延时有助于稳定
	SendStr("ready ok !",10);//串口发送
	while(1)         //主循环
	{
		if(reportFlag == 1)//定时上报处理
		{
			reportFlag = 0;	   //清零标志位
			if(baojignFlag == 1)  //需要报警
			{
				buzzer = !buzzer;  //蜂鸣器报警
			}
			else
			{
			 	buzzer = 1;	//蜂鸣器停止报警
			}
		
		
		}
	}
}

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
	TH0=(65536-20000)/256;		  //重新赋值 20ms
	TL0=(65536-20000)%256;
	EA=1;            //总中断打开
	ET0=1;           //定时器中断打开
	TR0=1;           //定时器开关打开
}
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz
	TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    ES    = 1;                  //打开串口中断
}

void SendByte(unsigned char dat)//串口发送单字节数据
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //将数据放入SBUF中
	while((!TI)&&(time_out<100))  //检测是否发送出去
	{time_out++;DelayUs2x(10);}	//未发送出去 进行短暂延时
	TI = 0;						//清除ti标志
}

void SendStr(unsigned char *s,unsigned char length)	   //发送定长度字符串
{
	unsigned char NUM;
	NUM=0x00;
	while(NUM<length)	//发送长度对比
	{
		SendByte(*s);  //放松单字节数据
		s++;		  //指针++
		NUM++;		  //下一个++
  	 }
}

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-20000)/256;		  //重新赋值 20ms
	TL0=(65536-20000)%256;
	times_20ms++;
	if(times_20ms%10==0)
	{
		reportFlag =1;					  //定时处理	
	}
}

void UART_SER (void) interrupt 4 	//串行中断服务程序
{
	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
		if(SBUF == '*')	//需要报警
		{
			baojignFlag = 1;
		}
		else if(SBUF == '#')	//不需要报警
		{
			baojignFlag = 0;
		}
	}
	if(TI)  //如果是发送标志位，清零
	TI=0;
} 
