/*
 * Project1_fan.c
 *
 * Created: 2025-02-21 오후 2:36:48
 * Author : USER
 */ 





//#define  F_CPU 16000000UL

#include "device/BUTTON.h"
#include "device/FND_1DIGIT.h"
#include "device/FAN.h"


int main(void)
{
	uint8_t FND_Number[]={0x6D,0x06,0x5B};
	uint8_t currentmode;
	
	BUTTON btn_STOP;
	BUTTON btn_MODE1;
	BUTTON btn_MODE2;
	FND fnd;
	FAN fan;
	
	buttonInit(&btn_STOP, &BUTTON_DDR, &BUTTON_PIN, BUTTON_STOP);
	buttonInit(&btn_MODE1, &BUTTON_DDR, &BUTTON_PIN, BUTTON_MODE1);
	buttonInit(&btn_MODE2, &BUTTON_DDR, &BUTTON_PIN, BUTTON_MODE2);
	fanInit(&fan,FAN_PIN);
	fndInit(&fnd);
	
	//Initializing
	fndOn(&fnd, FND_Number, (uint8_t)stop);
	currentmode = stop;
	/* Replace with your application code */
	while (1)
	{
		if(buttonGetState(&btn_STOP) == ACT_PUSH)
		{
			fndOn(&fnd, FND_Number, (uint8_t)stop);		
			setDuty(0);		
			currentmode = stop;
		}
	    else if(buttonGetState(&btn_MODE1) == ACT_PUSH)
		{
			fndOn(&fnd, FND_Number, (uint8_t)mode1);
			setDuty(100);
			_delay_ms(50);
			setDuty(50);
			currentmode = mode1;		
		}
		else if(buttonGetState(&btn_MODE2)==ACT_PUSH)
		{
			fndOn(&fnd, FND_Number, (uint8_t)mode2);
			setDuty(100);
			currentmode = mode2;
		}
		
	
		_delay_ms(10);
	}
}

