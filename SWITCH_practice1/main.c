/*
 * SWITCH_practice1.c
 *
 * Created: 2025-02-20 오전 9:11:19
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD =0xff;
	DDRG =0x00;
	
	uint8_t ledData =0x01;
	uint8_t buttonData;
	
    int flag=0; //integer in Atmega128a is 2byte
    /* Replace with your application code */
	
	PORTD=0x00;
	
    while (1) 
    {
		buttonData=PING;
		if((buttonData & (1<<4)) == 0)
		{
			PORTD=ledData;
			ledData=(ledData >> 7) | (ledData << 1);
			_delay_ms(300);
		}
		if((buttonData & (1<<3)) == 0)
		{
			PORTD=ledData;
			ledData = (ledData << 7) | (ledData >> 1);
			_delay_ms(300);
		}
		
		if(flag == 0)
		{
			if((buttonData & (1<<2))==0)
			{
				flag =1 ;
			}
			else
			{
				flag = 0 ;
			}
		}
		if(flag == 1)
		{
			for(uint8_t i=0; i <3;i++)
			{
				PORTD=0xff;
				_delay_ms(500);
				PORTD=0x00;
				_delay_ms(500);			
			}
			flag=0;
		}
    }
}

//need fix