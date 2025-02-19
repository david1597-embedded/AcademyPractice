/*
 * LED.c
 *
 * Created: 2025-02-19 오후 12:21:07
 *  Author: USER
 */ 
#include "LED.h"

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