#include "globals.h"
#include "display.h"

static u8 code codeTab[]={
// 0 1 2 3 4 5 6 7 8 9 []
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00, 0x40, 0x39};

u8 TubeBuf[TUBE_COUNT];
static u8 TubePos = 0;

static void ISR1() interrupt 1
{
	LatchControl(6, 0);
	LatchControl(7, ~codeTab[TubeBuf[TubePos]]);
	LatchControl(6, 1 << TubePos);
	TubePos++;
	TubePos %= TUBE_COUNT;
	
	SysTick++;
}

static void Timer0Init(void)	
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;
	TL0 = 0x66;
	TH0 = 0xEA;	
	TF0 = 0;
	TR0 = 1;
	IE0 = 1;
	ET0 = 1;
}

void InitTube()
{
	int i;
	for (i = 0; i < TUBE_COUNT; i++)
	{
		TubeBuf[i] = TubeEmpty;
	}
	Timer0Init();
}
