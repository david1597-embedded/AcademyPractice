/*
 * LED_bitarray.c
 *
 * Created: 2025-02-19 오후 2:48:11
 * Author : USER
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

uint8_t ledArr[]=
{
	0x00, //0b00000000
	0x80, //0b10000000
	0xC0, //0b11000000
	0xE0, //0b11100000
	0xF0, //0b11110000
	0xF8, //0b11111000
	0xFC, //0b11111100
	0xFE, //0b11111110
	0xFF, //0b11111111
	0x7F, //0b01111111
	0x3F, //0b00111111
	0x1F, //0b00011111
	0x0F, //0b00001111
	0x07, //0b00000111
	0x03, //0b00000011
	0x01  //0b00000001
};

int main(void)
{
	DDRD =0xff;
	uint8_t arrSize=sizeof(ledArr)/sizeof(ledArr[0]);
	/* Replace with your application code */
	while (1)
	{
		for(uint8_t i =0;i<arrSize;i++)
		{
			PORTD=ledArr[i];
			_delay_ms(200);
		}
	}
}
