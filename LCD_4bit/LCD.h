/*
 * LCD.h
 *
 * Created: 2025-03-07 오후 2:21:10
 *  Author: USER
 */ 




#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LCD_DATA_DDR  DDRC
#define LCD_DATA_PORT PORTC
#define LCD_RS_DDR    DDRB
#define LCD_RW_DDR    DDRB
#define LCD_E_DDR     DDRB
#define LCD_RS_PORT   PORTB
#define LCD_RW_PORT   PORTB
#define LCD_E_PORT    PORTB
#define LCD_RS_PIN     5
#define LCD_RW_PIN     6
#define LCD_E_PIN      7

#define COMMAND_DISPLAY_CLEAR 0x01
#define COMMAND_DISPLAY_ON    0x0C
#define COMMAND_DISPLAY_OFF   0x08
#define COMMAND_ENTRY_MODE    0x06
#define COMMAND_8BIT_MODE     0x38
#define COMMAND_4BIT_MODE     0x28

void LCD_DATA(uint8_t data); // 8bit
void LCD_DATA4Bit(uint8_t data);// 4bit
void LCD_WritePin();
void LCD_EnablePin();
void LCD_writeCommand(uint8_t commandData);
void LCD_WriteData(uint8_t charData);
void LCD_GotoXY(uint8_t row, uint8_t col);
void LCD_WriteString(char *string);
void LCD_WriteStringXY(uint8_t row, uint8_t col, char *string);
void LCD_Init();


#endif /* LCD_H_ */



