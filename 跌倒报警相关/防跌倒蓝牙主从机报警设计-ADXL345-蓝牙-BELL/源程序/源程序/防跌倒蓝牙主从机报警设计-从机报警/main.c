#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include  <math.h>    //Keil library  
#include "delay.h"

sbit buzzer=P2^0;

unsigned long times_20ms=0;		  	//��ʱ20ms��ʱ
unsigned char baojignFlag = 0;//������־
unsigned char reportFlag = 0;//��ʱ�ϱ�����

void Init_Timer0(void);//��������
void UART_Init(void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{
	Init_Timer0();        //��ʱ��0��ʼ��
	UART_Init();

	DelayMs(100);          //��ʱ�������ȶ�
	SendStr("ready ok !",10);//���ڷ���
	while(1)         //��ѭ��
	{
		if(reportFlag == 1)//��ʱ�ϱ�����
		{
			reportFlag = 0;	   //�����־λ
			if(baojignFlag == 1)  //��Ҫ����
			{
				buzzer = !buzzer;  //����������
			}
			else
			{
			 	buzzer = 1;	//������ֹͣ����
			}
		
		
		}
	}
}

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
	TH0=(65536-20000)/256;		  //���¸�ֵ 20ms
	TL0=(65536-20000)%256;
	EA=1;            //���жϴ�
	ET0=1;           //��ʱ���жϴ�
	TR0=1;           //��ʱ�����ش�
}
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz
	TL1 = TH1;  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    ES    = 1;                  //�򿪴����ж�
}

void SendByte(unsigned char dat)//���ڷ��͵��ֽ�����
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //�����ݷ���SBUF��
	while((!TI)&&(time_out<100))  //����Ƿ��ͳ�ȥ
	{time_out++;DelayUs2x(10);}	//δ���ͳ�ȥ ���ж�����ʱ
	TI = 0;						//���ti��־
}

void SendStr(unsigned char *s,unsigned char length)	   //���Ͷ������ַ���
{
	unsigned char NUM;
	NUM=0x00;
	while(NUM<length)	//���ͳ��ȶԱ�
	{
		SendByte(*s);  //���ɵ��ֽ�����
		s++;		  //ָ��++
		NUM++;		  //��һ��++
  	 }
}

void Timer0_isr(void) interrupt 1 
{
	TH0=(65536-20000)/256;		  //���¸�ֵ 20ms
	TL0=(65536-20000)%256;
	times_20ms++;
	if(times_20ms%10==0)
	{
		reportFlag =1;					  //��ʱ����	
	}
}

void UART_SER (void) interrupt 4 	//�����жϷ������
{
	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
		if(SBUF == '*')	//��Ҫ����
		{
			baojignFlag = 1;
		}
		else if(SBUF == '#')	//����Ҫ����
		{
			baojignFlag = 0;
		}
	}
	if(TI)  //����Ƿ��ͱ�־λ������
	TI=0;
} 
