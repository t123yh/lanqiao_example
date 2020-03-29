#include "globals.h"
#include "pwm.h"

u8 PWMRatio = 0;
#define MAX_COUNTER 10
static u8 counter = 0;

static void Timer1ISR() interrupt 3
{
	if (counter < PWMRatio)
	{
		P34 = 1;
	}
	else
	{
		P34 = 0;
	}
	counter++;
	counter %= MAX_COUNTER;
}

static void Timer1Init(void)		//100us@11.0592MHz
{
	AUXR |= 0x40;		//?????1T??
	TMOD &= 0x0F;		//???????
	TL1 = 0xAE;		//??????
	TH1 = 0xFB;		//??????
	TF1 = 0;		//??TF1??
	TR1 = 1;		//???1????
	IE1 = 1;
	ET1 = 1;
}


void InitPWM()
{
	Timer1Init();
}