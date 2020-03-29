#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <intrins.h>
#include <STC/STC15F2K60S2.H>
typedef char bool;
#define true 1
#define false 0
	
static void delay() { _nop_ (); _nop_ (); _nop_ (); }
typedef unsigned char u8;
typedef unsigned long u32;

extern bool KeyClickedMessage[];

void ProcessNumber(u8* number, u8 maxVal, bool plus);
void LatchControl(u8 latchNo, u8 value);
extern u32 SysTick;

#endif