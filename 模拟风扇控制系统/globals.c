#include "globals.h"

u32 SysTick = 0;

void LatchControl(u8 id, u8 val)
{
	bit savedEA = EA;
	EA = 0;
	Delay();
	P2 = P2 & 0x1F;
	Delay();
	P0 = val;
	Delay();
	P2 = P2 | (id << 5);
	Delay();
	P2 = P2 & 0x1F;
	Delay();
	EA = savedEA;
	Delay();
}