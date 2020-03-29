#ifndef __BUTTON_H
#define __BUTTON_H

#include "globals.h"
#define KEY_NUM 4

enum Buttons { 
	Time = 0, // P30
	Alarm = 1, // P31
	Plus = 2, // P32
	Minus = 3// P33
};

enum KeyStatus {
	None,
	Holding
};

void Key_Read_Routine(void);
void ClearButtonEvent();

#endif