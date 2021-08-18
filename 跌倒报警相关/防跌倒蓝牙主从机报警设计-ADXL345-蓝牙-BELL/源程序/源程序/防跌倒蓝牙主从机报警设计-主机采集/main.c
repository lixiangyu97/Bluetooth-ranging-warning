#include <reg52.h> 
#include <intrins.h>
#include <stdio.h>
#include  <math.h>    //Keil library  
#include "adxl345.h"
#include "delay.h"

sbit LED=P2^1;

unsigned long times_20ms=0;		  	//��ʱ20ms��ʱ
unsigned char Send_Message=0;	//������Ϣ��־
unsigned char ReadAdxl345=0;	//��ȡ��־
unsigned char Delay_Sec=0;		  //��ʱ��ʱ

unsigned int ErrorNum=0;	  //�������
unsigned int CheckNum=0;	  //������
char tab[16];

void Init_Timer0(void);//��������
void UART_Init(void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s,unsigned char length);

void main (void)
{
	Init_Timer0();        //��ʱ��0��ʼ��
	UART_Init();

	DelayMs(100);          //��ʱ�������ȶ�

	Init_ADXL345();	  //����
	if(Single_Read_ADXL345(0X00)==0xe5)	//����������Ϊ0XE5,��ʾ��ȷ
	{DelayMs(5);}
	else
	{DelayMs(3);}

	while(1)         //��ѭ��
	{
	    if(ReadAdxl345==1)   //��ʱ��ȡadxl345����
	    {
			ReadAdxl345=0;	 //��־����
			ReadData_x();  						//�����⺯��
			CheckNum++;
			if((temp_X<550)||(abs(temp_Y)>750))       //��λֵ�ж� �鿴��������
			{
				ErrorNum++;
			}
			if(CheckNum>=5)	  	//����5�δ���
			{
				if(ErrorNum>1)	   //�Ƕȳ��ִ��� 5�γ�Ϣ3�����
				{
					if(Send_Message==0)	  //�ϱ������ʼ��
					{
					 	Delay_Sec=0;	   //��ʱ����
					}
					Send_Message=1;		   //������Ϣ��־
					LED=0;
				}
				else
				{					
						Send_Message=0;		
						Delay_Sec=0;	 //��ʱ����
						LED=1;		 //�ر�
				}					
				ErrorNum=0;		  //����˲�����
				CheckNum=0;	
			}

			if((Send_Message==1)&&(Delay_Sec>=1))//2S �ϱ�
			{		
				SendStr("*****",5);//������Ϣ
			}
			else
			{
				SendStr("#####",5);//������Ϣ
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
		ReadAdxl345=1;					  //��ȡ��־��λ
		if(times_20ms%50==0)
		{
			if(Send_Message==1)		//��ʱ����
			{Delay_Sec++;}
		}		
	}
}

void UART_SER (void) interrupt 4 	//�����жϷ������
{
	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
	}
	if(TI)  //����Ƿ��ͱ�־λ������
	TI=0;
} 
