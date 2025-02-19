/*
 * LED_insideoutside_pointer.c
 *
 * Created: 2025-02-19 오후 12:18:55
 * Author : USER
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


#define LED_DDR DDRD
#define LED_PORT PORTD

void ledInit()
{
	DDRD=0xff;
	
}
void GPIO_Output(uint8_t data)
{
	LED_PORT=data;
}
void ledShift(uint8_t i,uint8_t *data)
{
	*data =(1 << i) | (1 << (7-i));
	
}
int main()
{
	ledInit();
	uint8_t ledData=0x81;
	
	while(1)
	{
		for(uint8_t i=0;i<8;i++)
		{
			ledShift(i,&ledData);
			GPIO_Output(ledData);
			_delay_ms(200);
		}
	}
}
