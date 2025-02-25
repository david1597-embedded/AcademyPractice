/*
 * Interrupt_practice8_usart_interrupt2.c
 *
 * Created: 2025-02-25 오후 3:33:57
 * Author : USER
 */ 

#include "UART0.h"

//FILE OUTPUT = FDEV_SETUP_STREAM(UART0_Transmit,NULL,_FDEV_SETUP_WRITE);

ISR(USART0_RX_vect)
{
	UART0_ISR_Process();
}

int main(void)
{
	UART0_Init();
	//stdout = &OUTPUT;
    /* Replace with your application code */
    while (1) 
    {
		UART0_execute();
    }
}

