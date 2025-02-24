/*
 * Interrupt_practice3_normalmode.c
 *
 * Created: 2025-02-21 오전 11:40:10
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
	
	DDRG |=(1<<0);
	PORTG &= ~(1<<0);
	
	TCCR0 |=(1<<CS02)|(1<<CS00);
	TCNT0 = 6;
	    /* Replace with your application code */
    while (1) 
    {
		while((TIFR & 0x01)==0);
				
		PORTG = ~PORTG;
		TCNT0 = 6;
		TIFR = 0x01;
		
    }
}

