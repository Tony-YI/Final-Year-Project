/*
#include <stdlib.h>
#include <stdio.h>
#include <string>
//for O_RDWR, O_NOCITTY, O_SYNC those MACRO
#include <fcntl.h>
#include "Serial.h"
*/
#include "main.h"
#include "XBEE.h"
#include "pprz_msg.h"
int main(int argc, char ** argv)
{
	char portname[32];
	if(argc < 2)
	{
		printf("Warning: Using default port /dev/ttyUSB0\n");
		strcpy(portname,"/dev/ttyUSB0");
	}
	else
		strcpy(portname,argv[1]);
	 
	printf("name of device %s\n",portname);
	#if _DEBUG
		printf("!!!DEBUG MODE\n");
	#endif
	#if _DEBUG_XBEE_parse_XBEE_msg
		printf("!!!DEBUGING XBEE_parse_XBEE_msg()\n");
	#endif


	//Configure the port	
	int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return 0;
	}
	set_interface_attribs(fd,B57600,0);
	set_blocking(fd,0);
	//Configuration done

	//declaring XBEE module
	XBEE xbee_coor(fd,0x00000000,0x0000ffff,0);
	while(1)
	{
		//read every 0.5s
		sleep(0.5);
		xbee_coor.XBEE_read_into_recv_buff();
		//after read, data in recv_buff should be parse by XBEE::XBEE_parse_XBEE_msg()
		xbee_coor.XBEE_parse_XBEE_msg();
		
		//show the message
		//xbee_coor.XBEE_show_msg();

		while(!xbee_coor.msg.empty())
		{
			XBEE_msg * ptr = xbee_coor.msg.front();
			xbee_coor.msg.pop();
			pprz_msg data = ptr->get_pprz_msg();
			//data.show_hex();	
			switch(data.pprz_get_msg_id())
			{
				case(FORWARD_MSG_ID_DL_SETTING):
					{
						printf("MSG: DL_SETTING\n");
						uint8_t cur_ac_id = 0, index = 0;
						float value = 0;
						data.pprz_get_DL_VALUE(cur_ac_id,index,value);
						printf("ac_id %d, index %d, value %f\n",cur_ac_id,index,value);	
					}
				case(RECV_MSG_ID_DL_VALUE):
					{
						uint8_t cur_ac_id = 0,index = 0;
						float value = 0;
						data.pprz_get_DL_VALUE(cur_ac_id,index,value);
						printf("MSG: DL_VALUE\n");
						printf("ac_id %d, index %d, value %f\n",cur_ac_id,index,value);
					}
					break;
				case(RECV_MSG_ID_ROTORCRAFT_STATUS):
					{
						struct ROTORCRAFT_STATUS quad_status;
						data.pprz_get_ROTORCRAFT_STATUS(quad_status);
						printf("MSG: ROTORCRAFT_STATUS\n");
						printf("ac_id %d,\nrc_status %d,\ngps_status %d,\nap_mode %d,\nap_inflight %d,\nap_motors_on %d\n\n",
								quad_status.ac_id,\
								quad_status.rc_status,\
								quad_status.gps_status,\
								quad_status.ap_mode,\
								quad_status.ap_inflight,\
								quad_status.ap_motors_on);
					}
					break;
				case(RECV_MSG_ID_ALIVE):
					{
						printf("MSG: ALIVE\n\n");
					}
					break;
				case(RECV_MSG_ID_ROTORCRAFT_NAV_STATUS):
					{
						struct ROTORCRAFT_NAV_STATUS quad_nav_status;
						data.pprz_get_ROTORCRAFT_NAV_STATUS(quad_nav_status);
						printf("MSG: ROTORCRAFT_NAV_STATUS\n");
						printf("ac_id %d\n,block_time %d\n,stage_time %d\n,cur_block %d\n,cur_stage %d\n\n",\
								quad_nav_status.ac_id,\
								quad_nav_status.block_time,\
								quad_nav_status.stage_time,\
								quad_nav_status.cur_block,\
								quad_nav_status.cur_stage);
					}
					break;
				case(RECV_MSG_ID_quad_swarm_ack):
					{
						printf("MSG: quad_swarm_ack\n");
						uint8_t ac_id, quad_swarm_id, quad_swarm_ack;
						data.pprz_get_quad_swarm_ack(ac_id,quad_swarm_id,quad_swarm_ack);	
						printf("ac_id %d,\nquad_swarm_id %d,\nquad_swarm_ack %d\n\n",ac_id,quad_swarm_id,quad_swarm_ack);
					}
					break;
				default:
					{
						printf("MSG_ID %d does not match any\n\n",data.pprz_get_msg_id());
					}
					break;
			}
			delete ptr;
		}
	}
	return 0;
}
