#include <stdlib.h>
#include <stdio.h>
#include <string>
//for O_RDWR, O_NOCITTY, O_SYNC those MACRO
#include <fcntl.h>
#include "Serial.h"
#include "XBEE.h"

int main(int argc, char ** argv)
{
	char portname[32];
	if(argc < 2)
	{
		printf("Warning: Using default port /dev/ttyUSB1\n");
		strcpy(portname,"/dev/ttyUSB1");
	}
	else
		strcpy(portname,argv[1]);
	
	printf("<This is a sample sending program>\n"); 
	printf("name of device %s\n",portname);
	#if _DEBUG
		printf("!!!DEBUG MODE\n");
	#endif
	#if _DEBUG_XBEE_parse_XBEE_msg
		printf("!!!DEBUGING XBEE_parse_XBEE_msg()\n");
	#endif
	int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return 0;
	}
	set_interface_attribs(fd,B57600,0);
	set_blocking(fd,0);
	XBEE xbee_coor(fd,0x00000000,0x0000ffff,0);
	//char dummy[32];
	const char data[] = "hello A!"; 
	uint32_t addr_hi = 0x0013a200;
	uint32_t addr_lo = 0x409c27b4;
	uint8_t net_addr_hi = 0xFF;
	uint8_t net_addr_lo = 0xFE;

	
	XBEE_msg msg;
	msg.set_tran_packet(addr_hi,addr_lo,net_addr_hi,net_addr_lo,(uint8_t *)data,strlen(data) + 1);	
	
	XBEE_msg msg1;
	char data2[] = "???";
	msg1.set_tran_packet(addr_hi,addr_lo,net_addr_hi,net_addr_lo,(uint8_t *)data2,strlen(data2) + 1);
	
	while(1)
	{
		sleep(2);
		//msg.show_hex();
		xbee_coor.XBEE_send_msg(msg);
		sleep(1);
		xbee_coor.XBEE_send_msg(msg1);
	}
	return 0;
}
