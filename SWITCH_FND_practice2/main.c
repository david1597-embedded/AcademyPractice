/*
 * SWITCH_FND_practice2.c
 *
 * Created: 2025-02-20 오후 2:00:40
 * Author : USER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define FND_DATA_DDR DDRB
#define FND_SELECT_DDR DDRC
#define FND_DATA_PORT PORTB
#define FND_SELECT_PORT PORTC

void fndDisPlay(uint16_t data)
{
	static uint8_t position=0;
	uint8_t fndData[]={	
		0x3F,
		0x06,
		0x5B,
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x27,
		0x7F,
		0x67
		};
	switch(position)
	{
		//set only digit 1 LED on
		case 0:
		FND_SELECT_PORT &= ~(1<<0);
		FND_SELECT_PORT |= (1<<1)|(1<<2)|(1<<3);
		FND_DATA_PORT = fndData[data/1000];
		break;
		
		case 1:
		FND_SELECT_PORT &= ~(1<<1);
		FND_SELECT_PORT |= (1<<0)|(1<<2)|(1<<3);
		FND_DATA_PORT = fndData[data/100%10];
		break;
		
		case 2:
		FND_SELECT_PORT &= ~(1<<2);
		FND_SELECT_PORT |= (1<<0)|(1<<1)|(1<<3);
		FND_DATA_PORT = fndData[data/10%10];
		break;
		
		case 3:
		FND_SELECT_PORT &= ~(1<<3);
		FND_SELECT_PORT |= (1<<0)|(1<<1)|(1<<2);
		FND_DATA_PORT = fndData[data%10];
		break;	
	}
	position ++;
	position = position %4;
}
int main(void)
{
	
    FND_DATA_DDR=0xff;
	FND_SELECT_DDR=0xff;
	FND_SELECT_PORT=0x00;
	
	uint16_t count=0;
	uint32_t timeTick=0;
	uint32_t prevTime=0;
    /* Replace with your application code */
    while (1) 
    {
        fndDisPlay(count);
		if(timeTick-prevTime > 100)//Increase count 
		{
			prevTime=timeTick;
			count ++;
		}
		_delay_ms(1); // 1ms delay
		timeTick++; // 시간을 1ms씩 증가
		if(count == 99)
		{
			count = 0;
		}
    }
}

