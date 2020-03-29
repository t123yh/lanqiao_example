#include "globals.h"

u32 SysTick = 0;

void LatchControl(u8 latchNo, u8 value)
{
	bit savedEA = EA;
	EA = 0;
	
  delay();
	P2 = P2 & 0x1F;
	delay();
	P0 = value;
	P2 = P2 | (latchNo << 5);
	delay();
	P2 = P2 & 0x1F;
	delay();
	
	EA = savedEA;
	delay();
}

void ProcessNumber(u8* number, u8 maxVal, bool plus)
{
	if (plus)
	{
		(*number)++;
		(*number) %= maxVal;
	}
	else
	{
		if (*number != 0)
			(*number)--;
		else
			(*number) = maxVal - 1;
	}
}