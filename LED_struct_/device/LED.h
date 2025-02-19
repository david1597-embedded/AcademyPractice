/*
 * LED.h
 *
 * Created: 2025-02-19 오전 10:22:59
 *  Author: USER
 */ 


#ifndef LED_H_
#define LED_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

typedef struct  
{
	volatile uint8_t *port; // port LED connected
	uint8_t           pin; // pin number LED connected
}LED;

void ledInit(LED *led);
void ledOn(LED *led);
void ledOff(LED *led);


#endif /* LED_H_ */