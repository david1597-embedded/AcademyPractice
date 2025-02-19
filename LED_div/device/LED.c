/*
 * LED.c
 *
 * Created: 2025-02-19 오전 9:56:27
 *  Author: USER
 */ 
#include "LED.h"

void ledInit()
{
	LED_DDR=0xff;
	
}
void GPIO_Output(uint8_t data)
{
	LED_PORT=data;
}
void ledLeftShift(uint8_t *data)
{
	*data=(*data >>7) | (*data <<1);
	GPIO_Output(*data);
}
void ledRightShift(uint8_t *data)
{
	*data=(*data <<7) | (*data >>1);
	GPIO_Output(*data);
}

