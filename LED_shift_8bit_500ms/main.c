/*
 * LED_shift_8bit_500ms.c
 *
 * Created: 2025-02-19 오후 12:09:09
 * Author : USER
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LEFT 0
#define RIGHT 1


int main(void)
{
	int direction=RIGHT;
	int start=1;
	DDRD = 0xff;
	PORTD=0x00;
	/* replace with your application code */
	while (1)
	{
		if(start == 1)
		{
			PORTD =0x01;
			_delay_ms(500);
			start = 2;
		}
		else if(start != 1)
		{

			if(direction == RIGHT)
			{
				for( uint8_t i=0; i<7 ;i++)
				{
					PORTD=(PORTD & 0xff) <<1;
					if(PORTD & ( 1<<PIND7))
					{
						direction=LEFT;
					}
					_delay_ms(500);
				}
			}
			else if(direction == LEFT)
			{
				for( uint8_t i=0; i<7 ;i++)
				{

					PORTD=(PORTD &0xff)>>1;
					if(PORTD & (1<<PIND0))
					{
						direction=RIGHT;
					}
					_delay_ms(500);
				}
			}
		}



	}
}
