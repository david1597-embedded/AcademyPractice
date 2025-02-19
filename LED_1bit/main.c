/*
 * LED_1bit.c
 *
 * Created: 2025-02-19 오후 12:07:06
 * Author : USER
 */ 

#define  F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0xFF;
	PORTD= 0x00;
	/* Replace with your application code */
	while (1)
	{
		PORTD =PORTD | (1<<PIND);
		_delay_ms(10);
		PORTD =PORTD & ~(1<<PIND0);
		_delay_ms(10);
	}
}


