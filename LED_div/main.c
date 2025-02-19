/*
 * LED_div.c
 *
 * Created: 2025-02-19 오전 9:32:00
 * Author : USER
 */ 
#include "device/LED.h"
int main(void)
{
	ledInit();
	uint8_t ledData=0x01;
	uint8_t start =1;
	
    /* Replace with your application code */
    while (1) 
    {
		if (start == 1)
		{
			LED_PORT=0x01;
			_delay_ms(200);
			start=99;
		}
		else if(start != 1)
		{
			
			for(uint8_t i=0;i<7;i++)
			{
				ledLeftShift(&ledData);
				_delay_ms(200);
			}
			for(uint8_t i=0;i<7;i++)
			{
				ledRightShift(&ledData);
				_delay_ms(200);
			}
		}
    }
}

