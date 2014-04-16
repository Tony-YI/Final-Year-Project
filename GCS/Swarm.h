#ifndef _SWARM_H
#define _SWARM_H

#include "main.h"
#include "address.h"
#include "geo.h"


#define QUAD_INIT 0
#define QUAD_RECV 1
#define QUAD_EXEC 2


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

#define LEADER_ID 1

class Swarm
{
	private:
		//the state machine of quadcopters
		uint8_t state[QUAD_NB + 1];
		//the XBEE address of quadcopters
		uint32_t *XBEE_ADDR_HI;
		uint32_t *XBEE_ADDR_LO;
		//the ECEF coordinate system position
		struct EcefCoor_i pos[QUAD_NB + 1];
		//the accruacy of positioning
		int32_t pacc[QUAD_NB + 1];
		//the last modified time
		uint64_t timestamp[QUAD_NB + 1];
		struct EcefCoor_i pos_mean_error;
	public:
		Swarm();
		~Swarm();
		uint32_t get_address_HI(uint8_t &AC_ID);
		uint32_t get_address_LO(uint8_t &AC_ID);
		uint8_t get_state(uint8_t &AC_ID);
		struct EcefCoor_i get_quad_coor(uint8_t &AC_ID);
		uint64_t get_timestamp(uint8_t &AC_ID);
		int32_t get_pacc(uint8_t &AC_ID);
		
		void set_quad_state(uint8_t &AC_ID, uint8_t s);
		void set_quad_ecef(uint8_t &AC_ID,struct EcefCoor_i &pos);
		void set_quad_pacc(uint8_t &AC_ID,int32_t paac);
		void set_pos_mean_error();
		
		bool all_in_state(uint8_t s);
};
#endif

