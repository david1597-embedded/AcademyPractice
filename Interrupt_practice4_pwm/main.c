/*
 * Interrupt_practice4_pwm.c
 *
 * Created: 2025-02-21 오후 2:18:29
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB|=(1<<DDRB4);
	
	TCCR0|=(1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS01)|(1<<CS00);
	
	//OCR0=127;
    /* Replace with your application code */
    while (1) 
    {
		for(uint8_t i=0;i<256;i++)
		{
			OCR0=i;
			_delay_ms(10);
		}
    }
}

