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

#ifndef QUAD_SWARM_H
#define QUAD_SWARM_H
#include "state.h"

//for autopilot_set_mode
#include "autopilot.h"

//for flight plan, nav_block, nav_stage etc
#include "subsystems/navigation/common_flight_plan.h"

/**********************************************************************/
/*** for transmission                                              ****/
/***DefaultChannel and DefaultDevice are 'ifndef then define here' ****/
/*** message.h is included here                                    ****/
/**********************************************************************/
#include "subsystems/datalink/downlink.h"

/**********************************************************************/
/****     UART2CheckFreeSpace is defined here                      ****/
/**********************************************************************/
#include "mcu_periph/uart.h"

//for autopilot_mode
#include "autopilot.h"

//for pacc of gps
#if USE_GPS
#include "subsystems/gps.h"
#endif
//for AC_ID
#include "generated/airframe.h"

//for block time, stage time
#include "firmwares/rotorcraft/navigation.h"

//for WAYPOINTS, MACRO OF generated flight_plan.h
#include "generated/flight_plan.h"
#define SWARM_INIT 0
#define SWARM_NEGOTIATE_REF 1
#define SWARM_WAIT_CMD 2
#define SWARM_WAIT_CMD_START_ENGINE 3
#define SWARM_WAIT_CMD_TAKEOFF 4
#define SWARM_WAIT_EXEC_ACK 5
#define SWARM_EXEC_CMD 6
#define SWARM_REPORT_STATE 7
#define SWARM_LANDHERE 8
#define SWARM_LANDED 9
#define SWARM_KILLED 10

#define SWARM_WP_FOR_USE 2

extern struct EcefCoor_i quad_swarm_target;
extern struct EnuCoor_i quad_swarm_original_wp;

extern void quad_swarm_init( void );
extern void quad_swarm_periodic( void );
extern void quad_swarm_event( void );
extern uint8_t quad_swarm_reach_tar( void );
extern void quad_swarm_target_to_waypoint( void );

extern uint8_t quad_swarm_state;
extern uint8_t quad_swarm_id;
extern uint8_t quad_swarm_recv_ack;

//called when DL_quad_swarm_msg is detected

#define quad_swarm_datalink(){\
	uint8_t ac_id = DL_quad_swarm_msg_ac_id(dl_buffer);\
	if(quad_swarm_id == ac_id && (quad_swarm_state == SWARM_WAIT_CMD_TAKEOFF || quad_swarm_state == SWARM_REPORT_STATE))\
	{\
		uint8_t dummy = DL_quad_swarm_msg_dummy(dl_buffer);\
		if(ac_id == AC_ID)\
		{\
		quad_swarm_target.x = DL_quad_swarm_msg_tar_ecef_pos_x(dl_buffer);\
		quad_swarm_target.y = DL_quad_swarm_msg_tar_ecef_pos_y(dl_buffer);\
		quad_swarm_target.z = DL_quad_swarm_msg_tar_ecef_pos_z(dl_buffer);\
		}\
		quad_swarm_state = SWARM_WAIT_EXEC_ACK;\
	}\
}

#define quad_swarm_ack_datalink(){\
	uint8_t ac_id = DL_quad_swarm_ack_forwarded_ac_id(dl_buffer);\
	uint8_t ack = DL_quad_swarm_ack_forwarded_ack(dl_buffer);\
	DOWNLINK_SEND_DL_VALUE(DefaultChannel,DefaultDevice,&ac_id,&ack);\
	if(ack == 0xff)\
	{\
		autopilot_set_mode(AP_MODE_KILL);\
		quad_swarm_state = SWARM_KILLED;\
	}\
	else if(ack == 0xfe)\
	{\
		autopilot_set_mode(AP_MODE_NAV);\
		quad_swarm_state = SWARM_INIT;\
		nav_init();\
	}\
	else if(ack == 2 && quad_swarm_state == SWARM_NEGOTIATE_REF)\
	{\
		quad_swarm_state = SWARM_WAIT_CMD;\
	}\
	else if(ack == 0xfd && quad_swarm_state == SWARM_WAIT_EXEC_ACK)\
	{\
		quad_swarm_state = SWARM_EXEC_CMD;\
		quad_swarm_target_to_waypoint();\
	}\
}

#define send_nav_info() DOWNLINK_SEND_ROTORCRAFT_NAV_STATUS(\
		DefaultChannel,\
		DefaultDevice,\
                   &block_time,				\
                   &stage_time,				\
                   &nav_block,				\
                   &nav_stage,				\
                   &horizontal_mode)

extern void quad_swarm_start( void );
extern void quad_swarm_stop( void );
#endif

