/*
 * Interrupt_practice6_usart.c
 *
 * Created: 2025-02-25 오전 11:30:49
 * Author : USER
 */ 

#include "UART0.h"




int main(void)
{
	UART0_Init();
	DDRD=0xff;
	PORTD=0x00;
	
	char data;
    /* Replace with your application code */
    while (1) 
    {
		data = UART0_Receive();
		//UART0_Transmit(UART0_Receive());	
		
		if(data == 'a')
		{
			PORTD=0xff;
		}
		else if(data == 'b')
		{
			PORTD=0x00;
		}	
		else if(data == 'c')
		{
			for(uint8_t i =0 ;i<8;i++)
			{
				
				PORTD |= (1<<i);
				_delay_ms(200);
			}
			PORTD=0x00;
		}
    }
}

