/*
 * FAN.c
 *
 * Created: 2025-02-22 오후 12:05:33
 *  Author: USER
 */ 


#include "FAN.h"

void fanInit(FAN *fan,uint8_t pinNum_param)
{
	TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS02)|(1<<CS01);
	fan->port=&FAN_PORT;
	fan->pinNum=pinNum_param;
	*(fan->port -1) |= (1<<fan->pinNum);
	
}
void setDuty(uint8_t duty_to_set)
{
	OCR0=255*duty_to_set/100;
}