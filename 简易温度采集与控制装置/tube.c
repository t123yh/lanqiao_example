#include "globals.h"
#include "tube.h"

u8 code TubeTab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,0x00,0x40};

int TubeBuf[TUBE_COUNT];
static u8 displayPos = 0;

static void Timer0Int() interrupt 1
{
	LatchControl(6, 0);
	LatchControl(7, TubeTab[TubeBuf[displayPos]]);
	LatchControl(6, 1 << displayPos);

	displayPos++;
	displayPos %= TUBE_COUNT;
	
	SysTick++;
}

static void Timer0Init()
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x66;		//??????
	TH0 = 0xEA;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	IE0 = 1;
	ET0 = 1;
}

void InitTube()
{
	u8 i;
	for (i = 0; i < TUBE_COUNT; i++)
	{
		TubeBuf[i] = TubeEmpty;
	}
	Timer0Init();
}
