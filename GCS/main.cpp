#include "main.h"
#include "Ground_Station.h"
#if _USE_MBED

#include "./mbed/mbed.h"
#include "port.h"

#else

#include "Serial.h" 

#endif

#include "XBEE.h"


/*
void Test1() //TESTING BROADCASTING 32 BYTE WITH 1 HZ
{
        char message[32] = "123456789abcdef123456789abcdef";
        uint8_t count = 0;
        while(count++ < 5)
        {
            xbee_coor.transmit(0xff,message,strlen(message));
        }
}
*/

int main(int agrc, char ** argv) 
{

  while(1)
  {
	XBEE xbee_coor(1,0x13a200,0x4099278a,0);
  }
  return 0;
}    
