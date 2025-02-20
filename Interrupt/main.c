/*
 * Interrupt.c
 *
 * Created: 2025-02-20 오후 3:53:20
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t test = 0; //volatile

ISR (INT4_vect)
{
	PORTD ^=0xff;
	
}
int main(void)
{
	//Set I bit of SREG Register
	sei();
	//cli();
	EICRB |= (1<<ISC41) |(1<<ISC40) ;
	EIMSK |= (1<<INT4);
	
	DDRE &= ~(1<<4);
	DDRD = 0xff;
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

