#include "globals.h"
#include "button.h"

u8 ButtonEvent[BUTTON_COUNT];

/* enum ButtonStatus_t { None = 0, Holding };
static ButtonStatus_t Status[BUTTON_COUNT];*/

bool GetP33() { return !P33; }
bool GetP32() { return !P32; }
bool GetP31() { return !P31; }
bool GetP30() { return !P30; }

typedef bool (*GetFunc)();

static GetFunc ReadKey[] = { GetP33, GetP32, GetP31, GetP30 };
static u16 ButtonHeldTime[BUTTON_COUNT];

void ButtonRoutine()
{
	int i;
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		if (ReadKey[i]())
		{
			ButtonHeldTime[i]++;
		}
		else
		{
			if (ButtonHeldTime[i] > 5)
			{
				ButtonEvent[i] = 1;
			}
			ButtonHeldTime[i] = 0;
		}
	}
}

void ClearButtonEvent()
{
	int i;
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		ButtonEvent[i] = 0;
	}
}
