#include "globals.h"
#include "display.h"
#include "pwm.h"
#include "button.h"
#include "onewire.h"

enum { Sleep, Natural, Normal };
u8 WindMode = Normal;
u8 CurrentTimeMode = 1; // 0: 0s 1: 60s 2: 120s
u8 TemperatureFlag = 0; // 0: Normal mode 1: Temperature display
u32 RemainingSeconds = 60;

void HandlePWMAndDisplay()
{
	u8 modeCode = WindMode + 1, temp;
	if (RemainingSeconds == 0)
	{
		PWMRatio = 0;
		LatchControl(4, ~0);
	}
	else if (WindMode == Sleep)
	{
		PWMRatio = 2;
		LatchControl(4, ~1);
	}
	else if (WindMode == Natural)
	{
		PWMRatio = 3;
		LatchControl(4, ~2);
	}
	else if (WindMode == Normal)
	{
		PWMRatio = 7;
		LatchControl(4, ~4);
	}
	TubeBuf[0] = TubeHyphen;
	TubeBuf[2] = TubeHyphen;
	TubeBuf[3] = TubeEmpty;
	if (TemperatureFlag)
	{
		TubeBuf[1] = 4;
		TubeBuf[4] = TubeEmpty;
		temp = rd_temperature();
		TubeBuf[5] = temp / 10 % 10;
		TubeBuf[6] = temp % 10;
		TubeBuf[7] = TubeC;
	}
	else
	{
		TubeBuf[1] = modeCode;
		TubeBuf[4] = 0;
		TubeBuf[5] = RemainingSeconds / 100;
		TubeBuf[6] = RemainingSeconds / 10 % 10;
		TubeBuf[7] = RemainingSeconds % 10;
	}
}

void SwitchMode()
{
	WindMode++;
	WindMode %= 3;
}

void SwitchTime()
{
	CurrentTimeMode++;
	CurrentTimeMode %= 3;
	if (CurrentTimeMode == 0)
	{
		RemainingSeconds = 0;
	}
	else if (CurrentTimeMode == 1)
	{
		RemainingSeconds = 60;
	}
	else
	{
		RemainingSeconds = 120;
	}
}

int main()
{
	EA = 1;
	InitTube();
	InitPWM();
	
	while(1)
	{
		u32 tickBkp = SysTick;
		
		if (tickBkp % 100 == 0)
			HandlePWMAndDisplay();
		
		if (tickBkp % 20 == 0)
			ButtonRoutine();
		
		if (tickBkp % 2000 == 0)
		{
			if (RemainingSeconds > 0)
			{
				RemainingSeconds--;
			}
		}
		
		if (ButtonEvent[BtnMode])
		{
			ClearButtonEvent();
			SwitchMode();
		}
		
		if (ButtonEvent[BtnTime])
		{
			ClearButtonEvent();
			SwitchTime();
		}
		
		if (ButtonEvent[BtnStop])
		{
			ClearButtonEvent();
			RemainingSeconds = 0;
		}
		
		if (ButtonEvent[BtnTemp])
		{
			ClearButtonEvent();
			TemperatureFlag = !TemperatureFlag;
		}
		
		while (tickBkp == SysTick);
	}
}