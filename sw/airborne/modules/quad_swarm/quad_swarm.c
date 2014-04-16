/*
 * Copyright (C) Edward HUANG
 *
 * This file is part of paparazzi

 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "modules/quad_swarm/quad_swarm.h"





uint8_t quad_swarm_state;
uint8_t quad_swarm_recv_ack = 0;

//like the ac_id
uint8_t quad_swarm_id = AC_ID;

struct EcefCoor_i quad_swarm_target;
uint8_t quad_swarm_ack;
uint8_t quad_swarm_initilized = 0;
void quad_swarm_init( void ) 
{
	//NavSetGroundReferenceHere();
	//autopilot_set_mode(AP_MODE_NAV);
	quad_swarm_state = SWARM_KILLED;
	memset(&quad_swarm_target,0,sizeof(struct EcefCoor_i));	
	quad_swarm_id = AC_ID;
	quad_swarm_ack = 0;
	quad_swarm_recv_ack = 0;
	quad_swarm_initilized = 1;

}

//sending the ecef coordinate to the ground control station
//the unit is cm
#define send_quad_swarm_report()  {\
	if (!bit_is_set(state.pos_status, POS_ECEF_I))\
	stateCalcPositionEcef_i();\
	DOWNLINK_SEND_quad_swarm_report(\
        DefaultChannel,\
        DefaultDevice,\
        &quad_swarm_id,\
        &state.ecef_pos_i.x,\
        &state.ecef_pos_i.y,\
        &state.ecef_pos_i.z,\
        &quad_swarm_state,\
	&autopilot_mode,\
	&gps.pacc)\
}
#define send_quad_swarm_ack() DOWNLINK_SEND_quad_swarm_ack(\
	DefaultChannel,\
	DefaultDevice,\
	&quad_swarm_id,\
	&quad_swarm_ack\
	)
	
void quad_swarm_periodic( void )
{
	//send_nav_info();
	
	if(!quad_swarm_initilized)
	{
		quad_swarm_init();
		send_quad_swarm_report();
	}
	
	if(autopilot_mode != AP_MODE_NAV)
	{
		//quad_swarm_ack = 0 means this quadcopter is not ready in NAV mod
		quad_swarm_init();
		//send_quad_swarm_report();
	}

	if(nav_block == 11)
	{
		quad_swarm_state = SWARM_LANDHERE;
	}
	else if(nav_block == 12)
	{
		quad_swarm_state = SWARM_LANDED;
	}

	
	switch(quad_swarm_state)
	{
		case(SWARM_INIT):
		{
			send_quad_swarm_report();
			//0
			//wait for AP_NAV  to start the module
			//if the nav_block is 2 (which means that GPS has got 3D fixed)
			//go to next state SWARM_NEGOTIATE_REF.
			if(nav_block == 2 && autopilot_mode == AP_MODE_NAV)
			{
				quad_swarm_state = SWARM_NEGOTIATE_REF;
			}		
			break;
		}
		case(SWARM_NEGOTIATE_REF):
		{
			//1
			//module started
			//sending its gps position to the GCS
			//wait for GCS to ack this quad that GCS has choose a gps position as the reference
			//goto next state SWARM_WAIT_CMD when received ack=0x02 at this state.

			//Try to send the ECEF coordinate of current quadcopter's local tangent plane coorrdination
			//Not sure whether it would work fine. 
			DOWNLINK_SEND_quad_swarm_report(\
        									DefaultChannel,\
        									DefaultDevice,\
        									&quad_swarm_id,\
        									&state.ned_origin_i.ecef.x,\
        									&state.ned_origin_i.ecef.y,\
        									&state.ned_origin_i.ecef.z,\
        									&quad_swarm_state,\
											&autopilot_mode,\
											&gps.pacc);
			//
			break;
		}
		case(SWARM_WAIT_CMD):
		{	
			send_quad_swarm_report();
			//2 -- the quadcopter has negotiate the reference point
			//wait for command (nav_block) to start engine	
			if(nav_block == 3) 
			{
				//the quad has started engine but not yet takeoff
				quad_swarm_state = SWARM_WAIT_CMD_START_ENGINE;
			}
			break;
		}
		case(SWARM_WAIT_CMD_START_ENGINE):
		{
			send_quad_swarm_report();
			//3 -- the quacopter has started engine.
			//wait for command (nav_block) to takeoff.
			if(nav_block == 4)
			{
				quad_swarm_state = SWARM_WAIT_CMD_TAKEOFF;
			}
			break;
		}
		case(SWARM_WAIT_CMD_TAKEOFF):
		{
			send_quad_swarm_report();
			//4 -- the quadcopter has taken off.
			//at this state, the quadcopetr is waiting for a command message from the GCS.
			//the message is dealt with in quad_swram.h quad_swarm_datalink() MACRO.
			break;
		}
		case(SWARM_WAIT_EXEC_ACK):
		{
			send_quad_swarm_report();
			//STATE 6
			//wait for ack to carry out the previous cmd
			break;
		}
		case(SWARM_EXEC_CMD):
		{
			send_quad_swarm_report();
			//STATE 7
			//change the target position of the navigation system
			//check the the execution status
			//if reach the target
			//proceed to the next state
			if(quad_swarm_reach_tar())
			{
				quad_swarm_state = SWARM_REPORT_STATE;
			}
			break;
		}
		case(SWARM_REPORT_STATE):
		{
			send_quad_swarm_report();
			//STATE 8
			//report that this quad has finished the task
			//if the quad received quad_swarm_msg, 
			//it will go back to SWARM_SEND_ACK
			break;
		}
		case(SWARM_LANDHERE):
		{
			send_quad_swarm_report();
		}
		break;
		case(SWARM_LANDED):
		{
			send_quad_swarm_report();
		}
		break;
		case(SWARM_KILLED):
		{
			send_quad_swarm_report();
			//STATE 9
			//quad_swarm_initilized = 0;
			
		}
		break;
		default:
		{
			send_quad_swarm_report();
			//Encounter Error, please handle
			break;
		}
	}	
}

void quad_swarm_event( void ) {}
uint8_t quad_swarm_reach_tar()
{
	return 1;			
}

void quad_swarm_target_to_waypoint()
{
	struct EnuCoor_i tmp;
	//the waypoints[SWARM_WP_FOR_USE] will be reset by nav_init(), which is called when a 
	//ack=0xfe is received, for reseting the quad_swarm module to SWARM_INIT
	enu_of_ecef_pos_i(&tmp,&ins_ltp_def,&quad_swarm_target);

	//DON'T CHANGE THE Z REFERENCE
	navigation_target.x = tmp.x;
	navigation_target.y = tmp.y;

	waypoints[SWARM_WP_FOR_USE].x = tmp.x;
	waypoints[SWARM_WP_FOR_USE].y = tmp.y;
	//navigation_target may be set to waypoints[WP_NB], so set the waypoint to the same value
	//enu_of_ecef_pos_i(&waypoints[SWARM_WP_FOR_USE],&ins_ltp_def,&quad_swarm_target);
	return ;
}
/*void quad_swarm_datalink( void )
{
	//handle the incoming message
	//find out whether it's the ack or real target command
	//and alter the state accordingly
}*/
void quad_swarm_start( void ){}
void quad_swarm_stop( void ){}
