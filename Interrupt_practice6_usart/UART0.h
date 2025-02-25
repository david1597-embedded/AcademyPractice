/*
 * UART0.h
 *
 * Created: 2025-02-25 오후 2:02:59
 *  Author: USER
 */ 


#ifndef UART0_H_
#define UART0_H_

#include "def.h"
void UART0_Init();

void UART0_Transmit(char data);

uint8_t UART0_Receive(void);




#endif /* UART0_H_ */