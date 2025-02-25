/*
 * Interrupt_practice5_16bitpwm.c
 *
 * Created: 2025-02-25 오전 9:32:25
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<DDRB5);
	
	TCCR1A |= (1<<COM1A1) | (1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | ( 1<<CS11) | (1<<CS10) ;
	ICR1=2499;
	
	OCR1A=1250;
    /* Replace with your application code */
    while (1) 
    {
		for (uint16_t i=0; i<2500;i++)
		{
			OCR1A=i;
			_delay_ms(1);
		}
    }
}

