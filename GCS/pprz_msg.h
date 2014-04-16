/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/
#ifndef _PPRZ_MSG_H
#define _PPRZ_MSG_H
#include "main.h"
#include "pprz_struct.h"

#define _IS_BIG_ENDIAN 0//this is not a big endian machine

/*****************************************************/
/*                received message id                */
/*****************************************************/
#define RECV_MSG_ID_DL_VALUE 31
#define RECV_MSG_ID_ROTORCRAFT_STATUS 231
#define RECV_MSG_ID_ALIVE 2
#define RECV_MSG_ID_ROTORCRAFT_NAV_STATUS 159
#define RECV_MSG_ID_quad_swarm_report 222
#define RECV_MSG_ID_quad_swarm_ack 223


/*****************************************************/
/*                forward message id                 */
/*****************************************************/
#define FORWARD_MSG_ID_BLOCK 5
#define FORWARD_MSG_ID_quad_swarm_msg 15
#define FORWARD_MSG_ID_quad_swarm_ack_forwarded 18
#define FORWARD_MSG_ID_DL_SETTING 4

#define DL_SETTING_TELEMETRY 0
/*****************************************************/
/*                     block id                      */
/*****************************************************/
#define BLOCK_ID_HOME_POINT 1
#define BLOCK_ID_HOLDING_POINT 2
#define BLOCK_ID_START_ENGINE 3
#define BLOCK_ID_TAKE_OFF 4
#define BLOCK_ID_LAND_HERE 11

#define PPRZ_MSG_MAX_SIZE 512
#define PPRZ_SENDER_ID_OFFSET 0
#define PPRZ_MSG_ID_OFFSET 1
#define PPRZ_DATA_OFFSET 2



class pprz_msg
{
	private:
		//pointer to the data
		uint8_t *_data_ptr;
		//_pos will be holding the current position to where the user has read
		uint16_t _pos;
		//_length will be holding the length of the current message
		uint16_t _length;
		uint8_t _errorcode;
		//_msg_id should be corresponding the records to messages.xml
		uint8_t _msg_id;
	public:
		pprz_msg();
		~pprz_msg();
		const uint8_t *pprz_get_data_ptr(){return _data_ptr;}
		const uint16_t pprz_get_length(){return _length;}
		
		void pprz_put_byte(uint8_t *ptr);
		void pprz_put_byte(uint8_t value);
		void pprz_put_2bytes(uint8_t *ptr);
		void pprz_put_4bytes(uint8_t *ptr);
		
		uint8_t pprz_read_byte();
		uint16_t pprz_read_2bytes();
		uint32_t pprz_read_4bytes();
		float pprz_read_float();
		void show_hex();
		void pprz_reset_msg();
		/************************************************/
		/*   member functions to set pprz message       */
		/************************************************/
		void pprz_set_block(uint8_t &ac_id,uint8_t &block_id);
		void pprz_set_DL_SETTING(uint8_t &ac_id, uint8_t &index, float &value);	
		void pprz_set_msg(uint8_t &ac_id,struct quad_swarm_msg &msg);		
		void pprz_set_ack(uint8_t &ac_id, uint8_t &ack);	

		/************************************************/
		/*   member functions to read pprz message      */
		/************************************************/
		uint8_t pprz_get_msg_id();
		
		void pprz_get_DL_VALUE(uint8_t &ac_id,uint8_t &index, float &value);
		
		void pprz_get_ROTORCRAFT_STATUS(struct ROTORCRAFT_STATUS &rotorcraft_status);
		void pprz_get_ALIVE();
		
		void pprz_get_ROTORCRAFT_NAV_STATUS(struct ROTORCRAFT_NAV_STATUS &rotorcraft_nav_status);

                void pprz_get_quad_swarm_ack(uint8_t &ac_id, uint8_t &quad_swarm_id, uint8_t &quad_swarm_ack);
                void pprz_get_quad_swarm_report(struct quad_swarm_report &report);


};

#endif
