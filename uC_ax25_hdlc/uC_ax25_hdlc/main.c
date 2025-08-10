/*
 * main.c
 *
 * Created: 7/20/2024 7:59:13 PM
 * Modified : 7/8/2025
 *  Author: DL8MCG
 */ 

#include <string.h>
#include <stdio.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "SerProg.h"
#include "board.h"
#include "AX25.h"
#include "timer.h"

#include "hdlc.h"

int main(void)
{
	int packetcnt = 0;

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

	//InitFSK(10006200,10007200);		// RF mark / space frequencies AD9833
	InitFSK(1200,2200);					// AF mark / space frequencies AD9833

	SerStart();
	while (isSer())
	{
		CallSmSer();
	}

	while(1)
	{
		AX25_Init(
					0x01,               // type AX.25 V2
					"QST", 0,           // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"ham radio test de nocall nr.%d", packetcnt++
				 );
	

		AX25_Init(
					//&ax25_frame,
					0x01,               // type AX.25 V2
					"QST", 0,           // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"the quick brown fox jumps over the lazy dog nr.%d", packetcnt++
				 );
	}
}