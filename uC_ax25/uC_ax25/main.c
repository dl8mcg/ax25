/*
 * main.c
 *
 * Created: 7/20/2024 7:59:13 PM
 *  Author: Hans
 */ 

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "SerProg.h"
#include "board.h"
#include "AX25.h"
#include "timer.h"

//Type : Command
//Source : NOCALL      SSID : 0
//Destination : QST    SSID : 0
//PID : 0xF0
//Control : 0x03
//The quick brown fox jumps over the lazy dog
const uint8_t  const AX25seqHDLC[] PROGMEM = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E,0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x45, 0x65, 0x15, 0x02, 0x02, 0x02, 0x07, 0x39, 0x79, 0x61, 0x41, 0x19, 0x19, 0x86, 0xC0, 0x0F, 0x2A, 0x16, 0xA6, 0x04, 0x8E, 0xAE, 0x96, 0xC6, 0xD6, 0x04, 0x46, 0x4E, 0xF6, 0xEE, 0x76, 0x04, 0x66, 0xF6, 0x1E, 0x04, 0x56, 0xAE, 0xB6, 0x0E, 0xCE, 0x04, 0xF6, 0x6E, 0xA6, 0x4E, 0x04, 0x2E, 0x16, 0xA6, 0x04, 0x36, 0x86, 0x5E, 0x9E, 0x04, 0x26, 0xF6, 0xE6, 0xA2, 0x73, 0x7E};

int main(void)
{
	wdt_enable(WDTO_2S);				// Watchdog auf 2 s setzen
	KEY_DDR_OUT;
	KEY_ON;
	GELB_DDR_OUT;
	ROT_DDR_OUT;

	// AX25 1200 bps  1200 Hz 2200 Hz
	cli();
	//init_timer_normal(1200);			// data rate in bits per second with normal-Timer
	init_timer_ctc(1200);				// data rate in bits per second with CTC-Timer
	SerInit();
		
	InitFSK(10005000,10006000);			// RF mark / space frequencies AD9833
	//InitFSK(1200,2100);				// AF mark / space frequencies AD9833

	SerStart();
	while (isSer())
	{
		CallSmSer();
	}
		
	InitAX25();
	
	while(1)
	{	
		SetAX25HDLC(AX25seqHDLC, sizeof(AX25seqHDLC));
    }
}