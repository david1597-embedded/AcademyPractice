/*
 * LED.h
 *
 * Created: 2025-02-19 오전 9:56:13
 *  Author: USER
 */ 


#ifndef LED_H_
#define LED_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

void ledInit();
void GPIO_Output(uint8_t data);
void ledLeftShift(uint8_t *data);
void ledRightShift(uint8_t *data);



#endif /* LED_H_ */