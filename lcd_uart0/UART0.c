/*
* UART0.c
*
* Created: 2025-02-25 오후 2:23:55
*  Author: USER
*/
#include "UART0.h"
#include "LCD.h"

uint8_t rxBuff[100]={0};
uint8_t rxFlag;

//return flag
uint8_t UART0_getFlag()
{
	return rxFlag;
}

//check if UART0 is available
uint8_t UART0_avail()
{
	if(!(UCSR0A & (1<<RXC0)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//retrun rxBuff[String]
uint8_t *UART0_readRxBuff()
{
	return rxBuff;
}

//Initialization
void UART0_Init()
{
	UBRR0H = 0;
	UBRR0L = 0xCF;
	UCSR0A |= (1<<U2X0);//speed x2
	UCSR0B |= (1<<RXCIE0);
	//UCSR0B |= (1<<TXCIE0);
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	//Asynchrounous, 8 bit, parity bit disabled, stop bit 1
	//UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	sei();
}

void UART0_clearRxFlag()
{
	rxFlag=0;
}

void UART0_setReadyRxFlag()
{
	rxFlag=1;
}

//define process when entering ISR(USART_Rx_vect)
void UART0_ISR_Process()
{
	uint8_t rxData =UDR0;
	static uint8_t rxHead=0;
	
	if(rxData=='\n')
	{
		rxBuff[rxHead]=rxData;
		rxHead++;
		rxBuff[rxHead]='\0';
		rxHead=0;
		UART0_setReadyRxFlag();
	}
	else
	{
		rxBuff[rxHead]=rxData;
		rxHead++;
	}

}

void UART0_execute()
{
	if(UART0_getFlag())
	{
		UART0_clearRxFlag();
		uint8_t *rxString = UART0_readRxBuff();
	    
		LCD_writeCommand(COMMAND_DISPLAY_CLEAR);
		LCD_WriteStringXY(0,0,rxString);
		//printf(rxString); Needs output stream
		UART0_print(rxString);
	}
	_delay_ms(300);
}


void UART0_Transmit(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))); // empty?
	UDR0 = data;
}

void UART0_print(uint8_t *str)
{

	for(int i=0; str[i];i++)
	{
		UART0_Transmit(str[i]);
	}
	UART0_Transmit('\0');
}


unsigned char UART0_Receive()
{
	while(!(UCSR0A & (1<<RXC0))); // Waiting for data Receiving
	return UDR0;
}

