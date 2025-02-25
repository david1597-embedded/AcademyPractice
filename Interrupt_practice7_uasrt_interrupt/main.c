/*
 * Interrupt_practice7_uasrt_interrupt.c
 *
 * Created: 2025-02-25 오후 2:22:40
 * Author : USER
 */ 

#include <avr/io.h>
#include "UART0.h"
//Set output stream
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_Transmit,NULL,_FDEV_SETUP_WRITE);

char rxBuff[100]={0};
uint8_t rxFlag=0;

ISR(USART0_RX_vect)
{
	static uint8_t rxHead = 0;
	uint8_t rxData = UDR0;
	
	if(rxData == '\n' || rxData == '\r')
	{
		rxBuff[rxHead]='\0';
		rxHead =0;
		rxFlag =1; //문자열을 받았다고 플래그를 설정
	}
	else
	{
		rxBuff[rxHead]=rxData;
		rxHead++;
	}
}
int main(void)
{
	UART0_Init();
	uint8_t rxData;
	stdout= &OUTPUT;
	sei();
    /* Replace with your application code */
    while (1) 
    {
		if(rxFlag == 1)
		{
			rxFlag = 0;
			printf(rxBuff);
		}
    }
}

//출력이 쌓여있다가 툭 튀어나온다. 개행문자가 없어서 ASCII코드로 수신하지 않으면 쌓인다.