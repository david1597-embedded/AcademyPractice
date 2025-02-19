/*
 * LED_insideoutside.c
 *
 * Created: 2025-02-19 오후 12:13:37
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define  LED_DDR DDRD
#define  LED_PORT PORTD

int main()
{
	LED_DDR=0xff;
	
	while(1)
	{
		for(uint8_t i=0; i<4; i++)
		{
			LED_PORT=((0x01 << i) | ( 0x80 >>i));
			_delay_ms(1000);
		}
		for(uint8_t i=0; i<4; i++)
		{
			LED_PORT=((0x08 >> i) | ( 0x10 <<i));
			_delay_ms(1000);
		}
	}
}

