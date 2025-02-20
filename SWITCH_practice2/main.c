/*
 * SWITCH_practice2.c
 *
 * Created: 2025-02-20 오전 9:32:29
 * Author : USER
 */ 
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define BUTTON_DDR DDRG
#define BUTTON_PIN PING
#define BUTTON_ON 4
#define BUTTON_OFF 3
#define BUTTON_TOGGLE 2

enum 
{
	PUSHED,
	RELEASED
};
enum
{
	NO_ACT,
	ACT_PUSH,
	ACT_RELEASE
};

typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *pin;
	uint8_t        btnpin;
	uint8_t        prevState;
	}BUTTON; 
	
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



int main(void)
{
	LED_DDR=0xff;
	BUTTON btnOn;
	BUTTON btnOff;
	BUTTON btnToggle;
    
	buttonInit(&btnOn, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
	buttonInit(&btnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
	buttonInit(&btnToggle, &BUTTON_DDR, &BUTTON_PIN, BUTTON_TOGGLE);
	
	/* Replace with your application code */
    while (1) 
    {
		if(buttonGetState(&btnOn) == ACT_RELEASE)
		{
			LED_PORT=0xff;
		}
		else if(buttonGetState(&btnOff) == ACT_RELEASE)
		{
			LED_PORT=0x00;
		}
		if(buttonGetState(&btnToggle)==ACT_RELEASE)
		{
			LED_PORT^=0xff;
		}
    }
}

