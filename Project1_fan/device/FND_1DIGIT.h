/*
 * fnd_1digid.h
 *
 * Created: 2025-02-21 오후 2:37:19
 *  Author: USER
 */ 


#ifndef FND_1DIGID_H_
#define FND_1DIGID_H_

#include "COMMON.h"
#define FND_PORT PORTA;
typedef struct
{
	volatile uint8_t *port; // port LED connected	 // pin number LED connected
}FND;

void fndInit(FND *fnd);
void fndOn(FND *fnd,uint8_t *FND_Number, uint8_t index);
void fndOff(FND *fnd);
 


 //FND will show 's',0x06, /FND will show '1'  0x5B //FND will show '2'}


#endif /* FND_1DIGID_H_ */