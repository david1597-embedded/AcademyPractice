/*
 * SWITCH.h
 *
 * Created: 2025-02-21 오후 2:38:36
 *  Author: USER
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include "COMMON.h"
//Button Cotrol Pulling
#define BUTTON_DDR DDRG
#define BUTTON_PIN PING
#define BUTTON_MODE1 4
#define BUTTON_MODE2 3
#define BUTTON_STOP 2

enum
{
	PUSHED = 0,
	RELEASED = 1
};
enum
{
	NO_ACT = 0,
	ACT_PUSH = 1,
	ACT_RELEASE = 2
};

typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *pin;
	uint8_t       btnpin;
	uint8_t       prevState;
}BUTTON;

void buttonInit(BUTTON *button,volatile uint8_t *ddr_param, volatile uint8_t *pin_param, uint8_t pinNum_param);
uint8_t buttonGetState(BUTTON *button);


#endif /* BUTTON_H_ */