/*
 * LED_struct_.c
 *
 * Created: 2025-02-19 오전 10:58:17
 * Author : USER
 */ 
#include "device/LED.h"

int main()
{
	LED led;
	led.port=&PORTD;
	led.pin=0;
	ledInit(&led);
	
	while(1)
	{
		ledOn(&led);
		_delay_ms(300);
		ledOff(&led);
		_delay_ms(300);
		
	}
}

