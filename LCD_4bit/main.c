/*
 * LCD_4bit.c
 *
 * Created: 2025-03-07 오후 2:20:07
 * Author : USER
 */ 

#include <avr/io.h>


int main(void)
{
	LCD_Init();
	LCD_GotoXY(0,0);
	LCD_WriteString("good good");
	LCD_GotoXY(1,0);
	LCD_WriteString("BADD BDAG");
    /* Replace with your application code */
    while (1) 
    {
    }
}

