#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <intrins.h>
#include <STC/STC15F2K60S2.H>

typedef unsigned char u8;
typedef unsigned long u32;
typedef unsigned int u16;

typedef char bool;
#define true 1
#define false 0

extern u32 SysTick; // 1 tick = 0.5ms

static void Delay() { _nop_(); _nop_(); _nop_(); }
void LatchControl(u8 id, u8 val);

#endif