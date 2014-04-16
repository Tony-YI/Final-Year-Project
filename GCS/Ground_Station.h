/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/

#ifndef _GROUND_STATION_H
#define _GROUND_STATION_H
#include <gtk/gtk.h>
#include "GUI.h"
#include "main.h"
#include "XBEE.h"
#include "pprz_msg.h"
#include "Swarm.h"
#include "Serial.h"
#include "geo.h"

#define LINE 0
#define TRI 1
#define REC 2

#define ACK_TO_SWARM_WAIT_CMD 2
#define ACK_TO_SWARM_EXEC_CMD 3

#define MAX_TIME_RESENT 1000
        
enum GroundControlStation_state {GCS_INIT,\
		GCS_NEGOTIATE_REF,\
		GCS_WAIT_TO_START_ENGINE,\
		GCS_START_ENGINE,\
		GCS_WAIT_TO_TAKEOFF,\
		GCS_TAKEOFF,\
		GCS_WAIT_TO_SEND_CMD,\
		GCS_SEND_CMD,\
		GCS_WAIT_ACK,\
		GCS_SEND_EXEC_ACK,\
		GCS_WAIT_REPORT};

class Ground_Station
{
	private:
		static Swarm *Swarm_state;
		static XBEE *Com;
		static GroundControlStation_state GCS_state;
		static uint8_t Formation_type;
		static uint8_t leader_id;
		//---------geodetic information--------------//
		static struct LtpDef_i ref;
		static struct EcefCoor_i target[QUAD_NB + 1];
		static struct NedCoor_i ned_pos[QUAD_NB + 1];
		//-------------------------------------------//
		static void Send_Msg_Block(uint8_t &AC_ID, uint8_t &BLOCK_ID);

	public:
		static GUI* GCS_GUI;
		//Ground_Station();
		//This function will initilize the serial port for communication and the xbee module etc.
		Ground_Station(char * port_name,int argc, char ** argv);
		~Ground_Station();
		
/********************************************************************************************/
/*                                    MAIN STATE FUNCTIONS HERE                             */
/********************************************************************************************/
		
		//after this function is called, 
		//all quadcopters should be in SWARM_NEGOTIATE_REF
		//the navigation subsystem of quadcopter would be in block 2

		//static void *init_quadcopters(void * arg);
		//this function will try to initilize all the quadcopters to be in 
		static void *init_quadcopters( void * arg);
		static void *init_quadcopters_thread( void * arg);

		static void negotiate_ref();
		static void *end_negotiate(void *arg);
		static void *end_negotiate_thread(void *arg);

		static void *start_engine( void * arg);
		static void *start_engine_thread(void * arg);
		
		static void *takeoff(void * arg);
		static void *takeoff_thread(void * arg);
		
		static void *go_north(void * arg);
		static void *go_north_thread(void *arg);
		
		static void *send_exec_cmd_ack(void *arg);
		static void *send_exec_cmd_ack_thread(void *arg);

		static void calculating_target();
		static void sending_target();
		static void wait_cmd_ack();
		
		static void wait_report();
/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
		

/**********************************************************************************************/
/*							Functions for formation computation								  */
/**********************************************************************************************/

		//these four functions set the target variables to
		//diviates from the current reference point
		//[distance] variable is in cm.
		static void compute_go_direction(uint8_t ac_id, uint8_t distance, uint8_t direction);
		static void compute_go_north(uint8_t ac_id, uint8_t distance);
		static void compute_go_south(uint8_t ac_id, uint8_t distance);
		static void compute_go_east(uint8_t ac_id, uint8_t distance);
		static void compute_go_west(uint8_t ac_id, uint8_t distance);

		static void compute_stright_line_NS();
		static void compute_stright_line_WE();

/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
		//Command on the navigation_block
		//This function will ask quadcopters to takeoff

		static void nav_start_engine(uint8_t AC_ID);
		static void nav_start_engine();
		
		static void nav_takeoff(uint8_t AC_ID);
		static void nav_takeoff();
		
		//This function will ask the quadcopter to takeoff
		static void takeoff_quadcopters();
		static void takeoff_quadcopter(uint8_t AC_ID);
	
		//land here
		static void* nav_land_here(void *arg);
		static void* nav_land_here_thread(void *arg);


		//
		static void send_ack(uint8_t AC_ID, uint8_t ack);	
		static void ap_kill_quadcopter(uint8_t AC_ID);
		static void ap_kill_quadcopter();
		static void ap_nav_quadcopter(uint8_t AC_ID);
		static void wait_all_quads(uint8_t state);

		//send the target to the quadcopter
		static void send_target(uint8_t AC_ID, struct EcefCoor_i *tar);
		
		//update the state of corresponding quadcopter according to content of report
		static void update_on_quad_swarm_report(quad_swarm_report report);
		static void update_ned_coor_by_ecef_coor();
		static void update_ned_coor_by_ecef_coor(uint8_t AC_ID);
		
		static void * periodic_data_handle(void * arg);
		static gboolean update_GUI_quad_status_pthread(gpointer userdata);
};

#define GCS_SHOW_REPORT(report) {\
	printf("MSG: quad_swarm_report\n");\
	printf("quad %d: ap_mode %d state %d\n",report.ac_id,report.ap_mode,report.state);\
	printf("quad %d: x %d y %d z %d\n",report.ac_id,report.x,report.y,report.z);\
	printf("quad %d: pacc %d\n\n",report.ac_id,report.pacc);\
}

#endif
