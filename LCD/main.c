/*
 * LCD.c
 *
 * Created: 2025-03-07 오전 9:40:24
 * Author : USER
 */ 
#include "LCD.h"


int main(void)
{
	LCD_Init();
	LCD_GotoXY(0,0);
	LCD_WriteString("Hello LCD");
	LCD_GotoXY(1,0);
	LCD_WriteString("HELLO AVR");
	
	char buff[30];
	uint16_t count = 0;
    /* Replace with your application code */
    while (1) 
    {
        sprintf(buff,"count : %d ",count++);
		LCD_WriteStringXY(1,0,buff);
		_delay_ms(200);
		
    }
}

