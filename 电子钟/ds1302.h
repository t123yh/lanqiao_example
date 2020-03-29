#ifndef __DS1302_H
#define __DS1302_H
#include "globals.h"

typedef struct {
	u8 secondL:4;
	u8 secondH:3;
	u8 CH:1;
	
	u8 minuteL:4;
	u8 minuteH:3;
	u8 _:1;
	
	u8 hourL:4;
	u8 hourH:2;
	u8 __:1;
	u8 Mode1224:1;
	
	u8 dateL:4;
	u8 dateH:2;
	u8 ___:2;
	
	u8 monthL:4;
	u8 monthH:1;
	u8 ____:3;
	
	u8 day:3;
	u8 _____:5;
	
	u8 yearL:4;
	u8 yearH:4;
} DS1302Data;

void DS1302BurstRead(DS1302Data* buf);
void DS1302BurstWrite(const DS1302Data* buf);
void WriteDS1302Register(u8 address, u8 dat);
u8 ReadDS1302Register(u8 address);

#endif
