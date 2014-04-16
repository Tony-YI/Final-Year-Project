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
						printf("ac_id %d, index %d, value %f\n\n",cur_ac_id,index,value);
					}
					break;
				case(RECV_MSG_ID_ROTORCRAFT_STATUS):
					{
						struct ROTORCRAFT_STATUS quad_status;
						data.pprz_get_ROTORCRAFT_STATUS(quad_status);
						printf("MSG: ROTORCRAFT_STATUS\n");
						printf("ap_mode %d\n",quad_status.ap_mode);
						printf("link_imu_nb_err %d\n",quad_status.link_imu_nb_err);
						uint8_t sender_id = *(data.pprz_get_data_ptr());
						if(quad_status.ap_mode == 12)
						{
							pprz_msg pprz_set_ap;
							uint8_t ack = 255;
							pprz_set_ap.pprz_set_ack(sender_id,ack);
							XBEE_msg set_ap;	
							uint32_t addr_hi = 0x0013a200;
							uint32_t addr_lo = 0x409c278a;
							uint16_t net_addr_hi = 0xff;
							uint16_t net_addr_lo = 0xfe;
							set_ap.set_tran_packet(addr_hi,\
										addr_lo,\
										net_addr_hi,\
										net_addr_lo,\
										pprz_set_ap.pprz_get_data_ptr(),\
										pprz_set_ap.pprz_get_length());
							xbee_coor.XBEE_send_msg(set_ap);
						}
						else if(quad_status.ap_mode == 1)
						{
							pprz_msg pprz_set_ap;
							uint8_t ack = 254;
							pprz_set_ap.pprz_set_ack(sender_id,ack);
							XBEE_msg set_ap;
							uint32_t addr_hi = 0x0013a200;
                                                        uint32_t addr_lo = 0x409c278a;
                                                        uint16_t net_addr_hi = 0xff;
                                                        uint16_t net_addr_lo = 0xfe;
                                                        set_ap.set_tran_packet(addr_hi,\
                                                                                addr_lo,\
                                                                                net_addr_hi,\
                                                                                net_addr_lo,\
                                                                                pprz_set_ap.pprz_get_data_ptr(),\
                                                                                pprz_set_ap.pprz_get_length());
                                                        xbee_coor.XBEE_send_msg(set_ap);

						}
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
						/*
						printf("ac_id %d\n,block_time %d\n,stage_time %d\n,cur_block %d\n,cur_stage %d\n\n",\
								quad_nav_status.ac_id,\
								quad_nav_status.block_time,\
								quad_nav_status.stage_time,\
								quad_nav_status.cur_block,\
								quad_nav_status.cur_stage);
						*/
					}
					break;
				case(RECV_MSG_ID_quad_swarm_ack):
					{
						printf("MSG: quad_swarm_ack\n");
						uint8_t ac_id, quad_swarm_id, quad_swarm_ack;
						data.pprz_get_quad_swarm_ack(ac_id,quad_swarm_id,quad_swarm_ack);
						//if(ac_id == 0)	
						printf("ac_id %d, quad_swarm_id %d, quad_swarm_ack %d\n\n",ac_id,quad_swarm_id,quad_swarm_ack);
					}
					break;
				default:
					{
						printf("MSG_ID %d does not match any\n\n",data.pprz_get_msg_id());
						data.show_hex();
					}
					break;
			}
			delete ptr;
		}
	}
	return 0;
}
