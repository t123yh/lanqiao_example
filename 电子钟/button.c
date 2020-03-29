#include "globals.h"
#include "button.h"

bool KeyClickedMessage[KEY_NUM];

u8 ReadPin30(){return P30;}
u8 ReadPin31(){return P31;}
u8 ReadPin32(){return P32;}
u8 ReadPin33(){return P33;}

typedef u8 (*PinReadReg_t)(void);

PinReadReg_t KeyPins[] = {ReadPin30,ReadPin31,ReadPin32,ReadPin33};

	void ClearButtonEvent()
	{
		int i;
		for(i = 0; i < KEY_NUM;i++)
	{
		KeyClickedMessage[i] = false;
	}
	}

void Key_Read_Routine()
{
	static u8 KeyStatus[KEY_NUM];
	static u8 KeyHeldTime[KEY_NUM];
	static u8 KeyLongPressed[KEY_NUM];
	
	int i;
	
	for(i = 0; i < KEY_NUM;i++)
	{
	  if(KeyPins[i]() == 0)
		{
		   KeyStatus[i] = Holding;
		}
		else
		{
			if(KeyStatus[i] == Holding)
			{
			  KeyStatus[i] =  None; 
				KeyHeldTime[i] = 0;
				KeyLongPressed[i] = 0;
			}
		}
		
		if(KeyStatus[i] == Holding)
		{
			KeyHeldTime[i]++;
			if(KeyLongPressed[i])
			{
				if(KeyHeldTime[i] == 20)
				{
					KeyHeldTime[i] = 0;
					KeyClickedMessage[i] = 1;
				}
		  }
			else
			{
				if(KeyHeldTime[i] == 5)
				{
				  KeyClickedMessage[i] = 1;
				}
			  if(KeyHeldTime[i] == 100)
				{
				  KeyHeldTime[i] = 0;
					KeyLongPressed[i] = 1;
				}
			}
		}
	} 
}