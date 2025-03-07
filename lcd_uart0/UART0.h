/*
 * UART0.h
 *
 * Created: 2025-02-25 오후 2:23:47
 *  Author: USER
 */ 


#ifndef UART0_H_
#define UART0_H_

#include "def.h"

uint8_t UART0_getFlag();
uint8_t UART0_avail();
uint8_t *UART0_readRxBuff();//Return sentence Received including '/0'

void UART0_Init();
void UART0_clearRxFlag();
void UART0_setReadyRxFlag();
void UART0_ISR_Process();
void UART0_execute();
void UART0_Transmit(unsigned char data);
void UART0_print(uint8_t *str);

unsigned char UART0_Receive();


#endif /* UART0_H_ */