/*
 * lcd_uart0.c
 *
 * Created: 2025-03-07 오후 3:45:12
 * Author : USER
 */ 


#include "LCD.h"
#include "UART0.h"
ISR(USART0_RX_vect)
{
	UART0_ISR_Process();
}
int main(void)
{
	UART0_Init();
	LCD_Init();
    /* Replace with your application code */
    while (1) 
    {
	    UART0_execute();
	
			
	
    }
}

