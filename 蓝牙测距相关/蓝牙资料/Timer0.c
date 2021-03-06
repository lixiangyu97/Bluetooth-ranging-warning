/*
	STC89C52RC  定时器0模块

*/

#include "Timer0.h"


/*
  功能描述: 初始化定时器0
  函数参数: 无
  返回说明: 无 
*/

void Timer0_Init(void)	//初始化定时器0
{
	TMOD |= 0x01;	//设置为定时方式1，16位定时器，非外部触发模式
	//TMOD |= 0x02;//设置为定时方式2，8位自动重装，非外部触发模式
	
  TH0 = 0xDc;//设置定时时间，由晶振速度调节
  TL0 = 0x00;
    
  ET0 = 1;	//开定时器0中断
	
	EA = 1; 	//开启总中断
	
  TR0 = 1;	//开启定时器0计数

}


