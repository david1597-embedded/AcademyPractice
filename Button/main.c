/*
 * Button.c
 *
 * Created: 2025-02-19 오후 3:05:59
 * Author : USER
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD=0xff;
	
	DDRG &= ~((1<<4) | (1<<3) | (1<<2));
	
	//PORTA = 0xff;
    /* Replace with your application code */
    while (1) 
    {
		if(!(PING & (1<<4)))
		{
			PORTD=0x01;
		}
		else if(!(PING & (1<<3)))
		{
			PORTD=0x02;
		}
		else if(!(PING & (1<<2)))
		{
			PORTD=0x04;
		}
    }
}

