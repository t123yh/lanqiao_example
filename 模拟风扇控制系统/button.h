#ifndef __BUTTON_H_
#define __BUTTON_H_

#define BUTTON_COUNT 4
#include "globals.h"

enum { 
	BtnMode = 0, // S4 = P33
	BtnTime = 1, // S5 = P32
	BtnStop = 2, // S6 = P31
	BtnTemp = 3  // S7 = P30
};

extern u8 ButtonEvent[BUTTON_COUNT];

void ButtonRoutine();
void ClearButtonEvent();

#endif