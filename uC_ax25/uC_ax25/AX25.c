/*
 * AX25.c
 *
 * Created: 12.05.2024 21:18:55
 *  Author: HS
 */ 

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "AX25.h"
#include "board.h"
#include "SerProg.h"

#define lenAX25Buf 256

uint8_t AX25Buf[lenAX25Buf];

uint8_t character;
uint8_t AX25byte;

void AX25Stop();

void AX25GetNextCharacter();
void AX25SendNextBits();

bool isAX25();

uint8_t syncbyte;
uint8_t syncnt;
uint8_t bitcnt;
uint8_t bytecnt;

volatile bool freq;

void InitAX25()
{
	freq = 0;
	smFSK=AX25Stop;		
}

void SetAX25HDLC(const uint8_t * buf, uint8_t size)
{
	uint8_t i = 0;
	while(i<size)
	{
		AX25Buf[i++] = *buf++;
	}
	AX25Buf[i] = 0;
	character = 0;								// start of buffer
	smFSK = AX25GetNextCharacter;				// hole erstes Zeichen
	bytecnt = size;
	sei();
	while(isAX25())
	{
	}
}
		
void AX25Stop()
{

}

bool isAX25()
{
	wdt_reset();
	return !(smFSK == AX25Stop);
}

void AX25GetNextCharacter()
{
	if(!bytecnt)									// end of buffer ?
	{
		smFSK = AX25Stop;
		return;
	}	
	bytecnt--;
	AX25byte = AX25Buf[character++];				// get CW-pattern of next text-character
	
	
	if(!(AX25byte & 0x80))					// MSB first; if zero, toggle frequency
		CallSmFreq();
	
	AX25byte <<= 1;
	bitcnt = 1;
	smFSK = AX25SendNextBits;
}

void AX25SendNextBits()
{
	if(!(AX25byte & 0x80))					// MSB first; if zero, toggle frequency
		CallSmFreq();
			
	AX25byte <<= 1;
	bitcnt++;
	if(bitcnt >= 8)
		smFSK = AX25GetNextCharacter;
	return;
}



