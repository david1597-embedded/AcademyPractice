/*
 * Interrupt_practice2_TIMERCOUNTER_CTC.c
 *
 * Created: 2025-02-21 오전 9:59:00
 * Author : USER
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB |= (1<<4);
	TCCR0|=(1<<COM00)|(1<<WGM01)|(1<<CS02);
	OCR0=124;
    /* Replace with your application code */
    while (1) 
    {
		while((TIFR & 0x02) == 0)
		
		TIFR=0x02;//force clear due to no ISR
		OCR0=124;
		
		
    }
}

