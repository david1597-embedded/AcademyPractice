/*
 * LED.h
 *
 * Created: 2025-02-19 오후 12:20:58
 *  Author: USER
 */ 


#ifndef LED_H_
#define LED_H_

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

void ledInit();
void GPIO_Output(uint8_t data);
void ledShift(uint8_t i,uint8_t *data);


#endif /* LED_H_ */