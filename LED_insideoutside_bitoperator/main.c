/*
 * LED_insideoutside_bitoperator.c
 *
 * Created: 2025-02-19 오후 12:17:41
 * Author : USER
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main()
{
	DDRD=0xff;
	
	unsigned char L =0x01;
	unsigned char R =0x80;
	
	while(1)
	{
		PORTD = L | R;
		L <<= 1; // L=L<<1;
		if( L ==0)
		{
			L=0x01;
		}
		R >>=1;
		if(R==0)
		{
			R=0x80;
		}
		_delay_ms(1000);
	}
}

