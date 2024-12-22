#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000ul
#include <util/delay.h>

#include "uart.h"
#include "waveform.h"



#define BAUD_RATE 9600

int main(void) {
	WAVE_Init();
	
	sei();
	DDRB=0xff;
	while (1)
	{
		//squareWave(255,255);
		WAVE_MainFunction();
		//sineWave(255,1);
		//ramp(255,128);
		//triangleWave(255,1);
	}
	return 0; // never reached
}
