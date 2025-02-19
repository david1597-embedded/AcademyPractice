/*
* LED.c
*
* Created: 2025-02-19 오전 10:23:52
*  Author: USER
*/


#include "LED.h"

void ledInit(LED *led)
{
	//Set port to output
	*(led->port-1) |= (1<<led->pin); // port address -1 == DDRD , DDRD |=(1<<3); set DDRD to 1(output)
}
void ledOn(LED *led)
{
	*(led->port) |= (1<<led->pin);
	
}
void ledOff(LED *led)
{
	*(led->port) &= ~(1<<led->pin); //Set 0 in certain pin Absolutely
	
}