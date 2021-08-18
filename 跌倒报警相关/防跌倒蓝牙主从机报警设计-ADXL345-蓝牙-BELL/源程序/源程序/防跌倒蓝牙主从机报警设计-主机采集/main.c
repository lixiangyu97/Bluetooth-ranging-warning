#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include  <math.h>    //Keil library  
#include "adxl345.h"
#include "delay.h"

sbit LED=P2^1;

unsigned long times_20ms=0;		  	//定时20ms计时
unsigned char Send_Message=0;	//发送消息标志
unsigned char ReadAdxl345=0;	//读取标志
unsigned char Delay_Sec=0;		  //延时计时

unsigned int ErrorNum=0;	  //错误计数
unsigned int CheckNum=0;	  //检测次数
char tab[16];

void Init_Timer0(void);//函数声明
void UART_Init(void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{
	Init_Timer0();        //定时器0初始化
	UART_Init();

	DelayMs(100);          //延时有助于稳定

	Init_ADXL345();	  //清屏
	if(Single_Read_ADXL345(0X00)==0xe5)	//读出的数据为0XE5,表示正确
	{DelayMs(5);}
	else
	{DelayMs(3);}

	while(1)         //主循环
	{
	    if(ReadAdxl345==1)   //定时读取adxl345数据
	    {
			ReadAdxl345=0;	 //标志清零
			ReadData_x();  						//三轴检测函数
			CheckNum++;
			if((temp_X<550)||(abs(temp_Y)>750))       //方位值判断 查看正常次数
			{
				ErrorNum++;
			}
			if(CheckNum>=5)	  	//进行5次处理
			{
				if(ErrorNum>1)	   //角度出现错误 5次出息3次情况
				{
					if(Send_Message==0)	  //上报情况初始化
					{
					 	Delay_Sec=0;	   //延时清零
					}
					Send_Message=1;		   //发送信息标志
					LED=0;
				}
				else
				{					
						Send_Message=0;		
						Delay_Sec=0;	 //延时清零
						LED=1;		 //关闭
				}					
				ErrorNum=0;		  //清空滤波计数
				CheckNum=0;	
			}

			if((Send_Message==1)&&(Delay_Sec>=1))//2S 上报
			{		
				SendStr("*****",5);//发送信息
			}
			else
			{
				SendStr("#####",5);//发送信息
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
		ReadAdxl345=1;					  //读取标志置位
		if(times_20ms%50==0)
		{
			if(Send_Message==1)		//延时计数
			{Delay_Sec++;}
		}		
	}
}

void UART_SER (void) interrupt 4 	//串行中断服务程序
{
	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
	}
	if(TI)  //如果是发送标志位，清零
	TI=0;
} 
