/*
 * FAN.h
 *
 * Created: 2025-02-22 오후 12:04:30
 *  Author: USER
 */ 


#ifndef FAN_H_
#define FAN_H_


#include "COMMON.h"

#define FAN_PORT PORTB
#define FAN_PIN 4

enum mode
{
	stop,
	mode1,
	mode2
	};


typedef  struct{
	volatile uint8_t * port;
	uint8_t pinNum;
}FAN;

void fanInit(FAN *fan,uint8_t pinNum_param);
void setDuty(uint8_t duty_to_set);


#endif /* FAN_H_ */