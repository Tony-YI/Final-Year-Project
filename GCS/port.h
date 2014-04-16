#ifndef _PORT_H
#define _PORT_H
#include "mbed.h"

#define TX_PIN p9
#define RX_PIN p10


static Serial pc(USBTX, USBRX);
 
//static Serial xbee_coor(TX_PIN, RX_PIN);

void port_init();



#endif