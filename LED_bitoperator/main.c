/*
 * LED_bitoperator.c
 *
 * Created: 2025-02-19 오후 2:49:26
 * Author : USER
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	DDRD=0xff;
	while(1)
	{
		
		for(uint8_t i=0; i<16;i++)
		{
			PORTD=0xff00>>i;
			_delay_ms(1000);
		}
	}
}
