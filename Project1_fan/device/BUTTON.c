/*
 * SWITCH.c
 *
 * Created: 2025-02-21 오후 2:39:23
 *  Author: USER
 */ 

#include "BUTTON.h"

void buttonInit(BUTTON *button,volatile uint8_t *ddr_param, volatile uint8_t *pin_param, uint8_t pinNum_param)
{
	button->ddr=ddr_param;
	button->pin=pin_param;
	button->btnpin=pinNum_param;
	button->prevState=RELEASED; //Not pressed anything
	*button->ddr &= ~(1<<button->btnpin); //set button pin to input (bit operator A = A & ~(1<<k)
}

uint8_t buttonGetState(BUTTON *button)
{
	// 눌렀을떄 뭔일 나고 똇을 떄 뭔 일 날 것이다.
	uint8_t curState = *button -> pin & ( 1 << button->btnpin); //Search method by using bit operator if
	if(curState == PUSHED && (button -> prevState == RELEASED))//pushing when not pushing
	{
		_delay_ms(50);
		button->prevState=PUSHED;
		return ACT_PUSH;
	}
	else if((curState != PUSHED) && (button -> prevState == PUSHED))//releasing while pushing
	{
		_delay_ms(50);
		button->prevState=RELEASED;
		return ACT_RELEASE;
	}
	return NO_ACT;
}
