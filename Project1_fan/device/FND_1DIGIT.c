/*
 * fnd_1digit.c
 *
 * Created: 2025-02-21 오후 2:38:24
 *  Author: USER
 */ 
#include "FND_1DIGIT.h"

void fndInit(FND *fnd)
{	
	fnd->port=&FND_PORT;
	*(fnd->port-1) = 0xff; // port address -1 == DDRD , DDRD |=(1<<3); set DDRD to 1(output)
    //DDRA=0xff;
}
void fndOn(FND *fnd, uint8_t * FND_Number, uint8_t index)
{
	*(fnd->port) = FND_Number[index];
}
void fndOff(FND *fnd)
{
	*(fnd->port) = 0x00; //Set 0 in certain pin Absolutely
}