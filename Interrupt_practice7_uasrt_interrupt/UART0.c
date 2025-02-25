/*
 * UART0.c
 *
 * Created: 2025-02-25 오후 2:23:55
 *  Author: USER
 */ 
#include "UART0.h"
void UART0_Init()
{
	UBRR0H = 0;
	UBRR0L = 207;
	UCSR0A |= (1<<U2X0);//speed x2
	UCSR0B |= (1<<RXCIE0);
	//UCSR0B |= (1<<TXCIE0);
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	//Asynchrounous, 8 bit, parity bit disabled, stop bit 1
	//UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
}

void UART0_Transmit(char data)
{
	while(!(UCSR0A & (1<<UDRE0))); // empty?
	UDR0 = data;
}

uint8_t UART0_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); // Waiting for data Receiving
	return UDR0;
}