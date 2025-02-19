/*
 * LED_insidoutsid_fileseparation.c
 *
 * Created: 2025-02-19 오후 12:20:42
 * Author : USER
 */ 
#include "device/LED.h"

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
