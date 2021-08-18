#include <reg52.h>
#include "uart1.h"
#include "Timer0.h"

sbit Speaker_Pin=P2^0;
sbit LED_Pin=P0^0;


unsigned char MS100_Con=0;
unsigned char OneSec_Con=0;

void delay3s(void)   //�ӳ�3��
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
	Uart1_Init();//���ڳ�ʼ��
	Timer0_Init();//��ʱ����ʼ��
	while(1)
	{
		if(MS100_Con>=10)
		{
			MS100_Con=0;
			Uart1_SendOneByte(0xa0);//���ڷ���0xa0
		}
		if(Uart1_Receive_Change)
		{
			Uart1_Receive_Change=0;
			if(Uart1_Receive_Data==0xa0)//�����յ��������Ƿ���a0
			{
				OneSec_Con=0;
				Speaker_Pin=1;
				LED_Pin=~LED_Pin;//����LED��˸
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
  ��������: ��ʱ��0�жϺ���
  ��������: ��
  ����˵��: ��
*/

void Timer0_Interrupt(void) interrupt 1	//	��ʱ��0�жϺ���
{
	
	TH0 = 0xDc;	//���ö�ʱʱ�䣬�����ʼ��ʹ�õ��Ƕ�ʱ��ʽ2����Ҫ����
	TL0 = 0x00;
  MS100_Con++;
	OneSec_Con++;
}