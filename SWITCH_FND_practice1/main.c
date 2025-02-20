/*
 * SWITCH_FND_practice1.c
 *
 * Created: 2025-02-20 오전 11:45:28
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	uint8_t FND_Number[]=
	{
		0x3F,
		0x06,
		0x5B,
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x27,
		0x7F,
		0x67
	};
	
	uint16_t count=0;
	
	
	DDRA=0xff;
    /* Replace with your application code */
    
	while (1) 
    {
	   PORTA=FND_Number[count];
	   count= (count+1) % 10;
	   _delay_ms(500);
    }
}

