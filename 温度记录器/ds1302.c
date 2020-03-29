#include "globals.h"
#include "ds1302.h"

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;	

static void WriteDS1302Byte(u8 temp) 
{
	u8 i;
	for (i = 0; i < 8; i++)
	{ 
		SCK = 0;
		_nop_();
		_nop_();
		SDA = temp & 0x01;
		temp >>= 1;
		_nop_();
		_nop_();
		SCK = 1;
		_nop_();
		_nop_();
	}
	_nop_();
	_nop_();
}

static u8 ReadDS1302Byte()
{
	u8 temp = 0, i;
	for (i = 0; i < 8; i++) 	
 	{		
		SCK = 0;
		temp >>= 1;
		_nop_();
		_nop_();
 		if (SDA)
			temp |= 0x80;	
		_nop_();
		_nop_();
 		SCK = 1;
		_nop_();
		_nop_();
	}
	_nop_();
	_nop_();
	return temp;
}

static void Begin()
{
	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
  _nop_();
}

static void End()
{
	RST=0;
	_nop_();
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=0;
	_nop_();
	SDA=1;
	_nop_();
}

void WriteDS1302Register(u8 address, u8 dat)     
{
 	Begin();
 	WriteDS1302Byte(address);	
 	WriteDS1302Byte(dat);		
	End();
}

u8 ReadDS1302Register(u8 address)
{
 	u8 result;
 	Begin();
 	WriteDS1302Byte(address);
 	result = ReadDS1302Byte();
 	End();
	return result;
}

void DS1302BurstRead(DS1302Data* buf)
{
	u8* bufAddr = (u8*)buf;
	u8 i;
	Begin();
 	WriteDS1302Byte(0xBF);
	for (i = 0; i < sizeof(DS1302Data); i++)
	{
		*(bufAddr + i) = ReadDS1302Byte();
	}
	End();
}

void DS1302BurstWrite(const DS1302Data* buf)
{
	u8* bufAddr = (u8*)buf;
	u8 i;
	
	WriteDS1302Register(0x8E, 0x00);
	
	Begin();
 	WriteDS1302Byte(0xBE);
	for (i = 0; i < sizeof(DS1302Data); i++)
	{
		WriteDS1302Byte(*(bufAddr + i));
	}
	End();
	
	WriteDS1302Register(0x8E, 0x80);
}