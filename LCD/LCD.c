/*
 * LCD.c
 *
 * Created: 2025-03-07 오전 9:41:37
 *  Author: USER
 */ 

#include "LCD.h"

void LCD_DATA(uint8_t data)
{
	LCD_DATA_PORT=data;//데이터 계속 날리는 중
}
void LCD_WritePin()
{
	LCD_RW_PORT &= ~(1<<LCD_RW_PIN);//RW PIN LOW로 설정해서 쓰기모드 진입
}
void LCD_ReadPin()
{
	LCD_RW_PORT |= (1<<LCD_RW_PIN);//읽기모드 진입
}
void LCD_EnablePin()
{
	LCD_E_PORT &= ~(1<<LCD_E_PIN);//일단 로우
	LCD_E_PORT |=  (1<<LCD_E_PIN);//active high
	LCD_E_PORT &= ~(1<<LCD_E_PIN);//다시 로우
	_delay_ms(2);//시간이 모자르면 동작 안함
}
void LCD_writeCommand(uint8_t commandData)
{
	LCD_RS_PORT &= ~(1<<LCD_RS_PIN);//명령어 모드로 설정
	LCD_WritePin();
	LCD_DATA(commandData);
	LCD_EnablePin();
}
void LCD_WriteData(uint8_t charData)
{
	LCD_RS_PORT |= (1<<LCD_RS_PIN);//명령어 모드로 설정
	LCD_WritePin();
	LCD_DATA(charData);
	LCD_EnablePin();
}
void LCD_GotoXY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	uint8_t address = (0x40 * row) +col; //주소 계산
	uint8_t command = 0x80 + address; // 주소 설정
	LCD_writeCommand(command);
}
void LCD_WriteString(char *string)
{
	for(uint8_t i =0 ; string [i];i++)
	{
		LCD_WriteData(string[i]);	
	}
}
void LCD_WriteStringXY(uint8_t row, uint8_t col, char *string)
{
	LCD_GotoXY(row, col);
	LCD_WriteString(string);
}
void LCD_Init()
{
	LCD_DATA_DDR = 0xff;
	LCD_RS_DDR |= (1<<LCD_RS_PIN);
	LCD_RW_DDR |= (1<<LCD_RW_PIN);
	LCD_E_DDR |= (1<<LCD_E_PIN);
	
	_delay_ms(20);
	LCD_writeCommand(COMMAND_8BIT_MODE);
	_delay_ms(5);
	LCD_writeCommand(COMMAND_8BIT_MODE);
	_delay_ms(5);
	LCD_writeCommand(COMMAND_8BIT_MODE);
	LCD_writeCommand(COMMAND_8BIT_MODE);
	LCD_writeCommand(COMMAND_DISPLAY_OFF);
	LCD_writeCommand(COMMAND_DISPLAY_CLEAR);
	
	LCD_writeCommand(COMMAND_DISPLAY_ON);
	LCD_writeCommand(COMMAND_ENTRY_MODE);
}