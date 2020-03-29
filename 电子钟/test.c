#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "tube.h"
#include "ds1302.h"
#include "button.h"

DS1302Data currentEditing;
DS1302Data alarmTime;
DS1302Data currentTime;

u32 alarmStartTick;

enum
{
  GlobalSystemStatus_TimeDisplay,
	GlobalSystemStatus_TimeEdit,
	GlobalSystemStatus_AlarmEdit,
	GlobalSystemStatus_Alarming
};

u8 GlobalSystemStatus;
u8 SubSystemStatus = 0;

void ShowTimeData(const DS1302Data* dat, int currentItem)
{
	bool showCurrentSettingItem = SysTick % 500 > 250;
	
	if (currentItem == 0 && !showCurrentSettingItem)
		TubeBuf[0] = TubeBuf[1] = TubeEmpty;
	else
	{
		TubeBuf[0] = dat->hourH;
		TubeBuf[1] = dat->hourL;
	}
	TubeBuf[2] = TubeHyphen;
	
	if (currentItem == 1 && !showCurrentSettingItem)
		TubeBuf[3] = TubeBuf[4] = TubeEmpty;
	else
	{
		TubeBuf[3] = dat->minuteH;
		TubeBuf[4] = dat->minuteL;
	}
	TubeBuf[5] = TubeHyphen;
	
	if (currentItem == 2 && !showCurrentSettingItem)
		TubeBuf[6] = TubeBuf[7] = TubeEmpty;
	else
	{
		TubeBuf[6] = dat->secondH;
		TubeBuf[7] = dat->secondL;
	}
}

void ShowTime()
{
		ShowTimeData(&currentTime, -1);
}

void HandleAlarm()
{
	bool isPressed = false;
	int i;
	ShowTime();
	for (i = 0; i < KEY_NUM; i++)
	{
		if (KeyClickedMessage[i])
			isPressed = true;
	}
	ClearButtonEvent();
	
	if ((SysTick - alarmStartTick) % 800 > 400)
	{
		LatchControl(4, 0x7F);
	}
	else
	{
		LatchControl(4, 0xFF);
	}
	
	if (isPressed || SysTick - alarmStartTick > 2000 * 5)
	{
		LatchControl(4, 0xFF);
		GlobalSystemStatus = GlobalSystemStatus_TimeDisplay;
	}
}

void TimeEdit()
{
	ShowTimeData(&currentEditing, SubSystemStatus);
	if ((GlobalSystemStatus == GlobalSystemStatus_TimeEdit && KeyClickedMessage[Time]) ||
		(GlobalSystemStatus == GlobalSystemStatus_AlarmEdit && KeyClickedMessage[Alarm]))
	{
		SubSystemStatus++;
	}
	else switch(SubSystemStatus)
	{
	  case 0 : 
			if(KeyClickedMessage[Plus] || KeyClickedMessage[Minus])
			{
				u8 hour = currentEditing.hourL + currentEditing.hourH * 10;
				
				ProcessNumber(&hour, 24, KeyClickedMessage[Plus]);
				
				currentEditing.hourL = hour % 10;
				currentEditing.hourH = hour / 10;
				
			}
		break;
		case 1 : 
			if(KeyClickedMessage[Plus] || KeyClickedMessage[Minus])
			{
				u8 minute = currentEditing.minuteL + currentEditing.minuteH * 10;
				
				ProcessNumber(&minute, 60, KeyClickedMessage[Plus]);

				currentEditing.minuteL = minute % 10;
				currentEditing.minuteH = minute / 10;
				
			}
		break;
		case 2 :
			if(KeyClickedMessage[Plus] || KeyClickedMessage[Minus])
			{
				u8 second = currentEditing.secondL + currentEditing.secondH * 10;
				
				ProcessNumber(&second, 60, KeyClickedMessage[Plus]);

				currentEditing.secondL = second % 10;
				currentEditing.secondH = second / 10;
				
			}
			break;
		case 3:
			SubSystemStatus = 0;
			if (GlobalSystemStatus == GlobalSystemStatus_TimeEdit)
			{
				DS1302BurstWrite(&currentEditing);
			}
			else if (GlobalSystemStatus == GlobalSystemStatus_AlarmEdit)
			{
				alarmTime = currentEditing;
			}

			GlobalSystemStatus = GlobalSystemStatus_TimeDisplay;
			break;
	}
	ClearButtonEvent();
}

void main(void)
{
	int i, k;
	char buf[10];
	
	WriteDS1302Register(0x8E, 0x00);
	WriteDS1302Register(0x80, 0);

	EA = 1;
	InitTube();
	/*
	currentEditing = alarmTime;
	alarmTime.secondL = 5;
	currentEditing.secondL = 0;
	DS1302BurstWrite(&currentEditing);
	*/
	while(1)
	{
		u32 tickBkp = SysTick;
		
		if (SysTick % 40 == 0)
		{
			DS1302BurstRead(&currentTime);
					
			if (currentTime.secondL == alarmTime.secondL &&
					currentTime.secondH == alarmTime.secondH &&
					currentTime.minuteL == alarmTime.minuteL &&
					currentTime.minuteH == alarmTime.minuteH &&
					currentTime.hourL == alarmTime.hourL &&
					currentTime.hourH == alarmTime.hourH && 
					GlobalSystemStatus == GlobalSystemStatus_TimeDisplay)
			{
				alarmStartTick = SysTick;
				GlobalSystemStatus = GlobalSystemStatus_Alarming;
			}
		}
		
	  if (SysTick % 20 == 0)
		{
		  Key_Read_Routine();
		}
		
		if (KeyClickedMessage[Time] == 1 && GlobalSystemStatus == GlobalSystemStatus_TimeDisplay)
		{
			ClearButtonEvent();
			GlobalSystemStatus = GlobalSystemStatus_TimeEdit;
			SubSystemStatus = 0;
			DS1302BurstRead(&currentEditing);
		}
		
		if (KeyClickedMessage[Alarm] == 1 && GlobalSystemStatus == GlobalSystemStatus_TimeDisplay)
		{
			ClearButtonEvent();
			GlobalSystemStatus = GlobalSystemStatus_AlarmEdit;
			SubSystemStatus = 0;
			currentEditing = alarmTime;
		}
		
		switch(GlobalSystemStatus)
		{
			case GlobalSystemStatus_Alarming:
				HandleAlarm();
				break;
		  case GlobalSystemStatus_TimeDisplay:
				ShowTime();
				break;
			case GlobalSystemStatus_TimeEdit:
			case GlobalSystemStatus_AlarmEdit:
				TimeEdit();
				break;
		}
		
    while(tickBkp == SysTick);
	}
}
