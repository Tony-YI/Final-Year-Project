/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/

#include "pprz_msg.h"


pprz_msg::pprz_msg()
{
	this->_data_ptr = new uint8_t[PPRZ_MSG_MAX_SIZE];
	this->_length = 0;
	this->_pos = 0;
	this->_errorcode = 0;
}

pprz_msg::~pprz_msg()
{
	delete this->_data_ptr;
}


void pprz_msg::pprz_put_byte(uint8_t *ptr)
{
	if(_length + 1 > PPRZ_MSG_MAX_SIZE)
	{
		//SHOULD SET _errorcode
	}
	else
	{
		//printf("puting *ptr %02x\n",*ptr);
		*(this->_data_ptr + this->_length++) = *ptr;
	}
}

void pprz_msg::pprz_put_byte(uint8_t value)
{
	if(_length + 1 > PPRZ_MSG_MAX_SIZE)
        {
                //SHOULD SET _errorcode
        }
        else
        {
                //printf("puting *ptr %02x\n",*ptr);
                *(this->_data_ptr + this->_length++) = value;
        }

}
void pprz_msg::pprz_put_2bytes(uint8_t *ptr)
{
	#if _IS_BIG_ENDIAN
	// if big endian (say, apple MAC)
	// to make LSB comes first
	printf("big endian %d\n",BIG_ENDIAN);
	this->pprz_put_byte(ptr + 1);
	this->pprz_put_byte(ptr);	
	#else
	//LINUX
	//LSB comes first (pprz use this)
	//THIS IS LITTLE ENDIAN
	this->pprz_put_byte(ptr);
	this->pprz_put_byte(ptr + 1);
	#endif
}

void pprz_msg::pprz_put_4bytes(uint8_t *ptr)
{
	#if _IS_BIG_ENDIAN
	// if big endian (say, apple MAC)
	// to make LSB comes first
	printf("big endian %d\n",BIG_ENDIAN);
	this->pprz_put_byte(ptr + 3);
	this->pprz_put_byte(ptr + 2);
	this->pprz_put_byte(ptr + 1);
	this->pprz_put_byte(ptr);		
	#else
	//LINUX
	this->pprz_put_byte(ptr);
	this->pprz_put_byte(ptr + 1);
	this->pprz_put_byte(ptr + 2);
	this->pprz_put_byte(ptr + 3);
	#endif
}

uint8_t pprz_msg::pprz_read_byte()
{
	if(this->_pos >= this->_length)
	{
		//set error code when there's no data and the user want to read
		return 0;
	}
	else
	{
		//printf("reading *ptr %02x\n",*(this->_data_ptr + this->_pos));
		return *(this->_data_ptr + this->_pos++); 
	}
}

uint16_t pprz_msg::pprz_read_2bytes()
{
	//lsb comes first
	uint8_t LSB = this->pprz_read_byte();
	uint8_t MSB = this->pprz_read_byte();	
	return ((uint16_t)MSB << 8) | LSB;
}

uint32_t pprz_msg::pprz_read_4bytes()
{
	uint8_t B1 = this->pprz_read_byte();
	uint8_t B2 = this->pprz_read_byte();
	uint8_t B3 = this->pprz_read_byte();
	uint8_t B4 = this->pprz_read_byte();
	return B4 << 24 | B3 << 16 | B2 << 8 | B1;
}

float pprz_msg::pprz_read_float()
{
	if(_pos + (sizeof(float) - 1) >= this->_length )
	{
		printf("_pos too large\n");
		return 0;
	}
	else
	{
		//printf("1st byte %02x\n",*(_data_ptr+_pos));
		//printf("2nd byte %02x\n",*(_data_ptr+_pos+1));
		//printf("3rd byte %02x\n",*(_data_ptr+_pos+2));
		//printf("4th byte %02x\n",*(_data_ptr+_pos+3));
		union { uint32_t u; float f;} _f;
		_f.u =  (uint32_t)(*((uint8_t*)_data_ptr + _pos) |*((uint8_t*)_data_ptr+_pos+1)<< 8 |((uint32_t)*((uint8_t*)_data_ptr+_pos+2))<<16|((uint32_t)*((uint8_t*)_data_ptr+_pos+3))<<24);
		_pos += sizeof(float);
		return _f.f;	
	}
}
void pprz_msg::show_hex()
{
	uint16_t count = 0;
	while(count < this->_length)
	{
		printf("%02x ",*(this->_data_ptr + count++));
	}
	printf("\n");
}

void pprz_msg::pprz_reset_msg()
{
	_pos = 0;
}

/*********************************************/
/*    member functions to set pprz message   */
/*********************************************/
void pprz_msg::pprz_set_block(uint8_t &ac_id,uint8_t &block_id)
{
	//TODO Find out what should be the sender id here
	//sender id
	this->pprz_put_byte(&ac_id);	
	//message id
	this->pprz_put_byte((uint8_t)FORWARD_MSG_ID_BLOCK);
	//message content
	this->pprz_put_byte(&block_id);
	this->pprz_put_byte(&ac_id);	
}

void pprz_msg::pprz_set_DL_SETTING(uint8_t &ac_id,uint8_t &index, float &value)
{
	this->pprz_put_byte(&ac_id);
	this->pprz_put_byte((uint8_t)FORWARD_MSG_ID_DL_SETTING);
	this->pprz_put_byte(&index);
	this->pprz_put_byte(&ac_id);
	this->pprz_put_4bytes((uint8_t *)&value);
}

void pprz_msg::pprz_set_msg(uint8_t &ac_id, struct quad_swarm_msg &msg)
{
	this->pprz_put_byte(&ac_id);
	this->pprz_put_byte((uint8_t) FORWARD_MSG_ID_quad_swarm_msg);
	this->pprz_put_byte(&ac_id);
	uint8_t dummy = 0;
	this->pprz_put_byte(&dummy);
	this->pprz_put_4bytes((uint8_t *)&msg.x);
	this->pprz_put_4bytes((uint8_t *)&msg.y);
	this->pprz_put_4bytes((uint8_t *)&msg.z);	
}

void pprz_msg::pprz_set_ack(uint8_t &ac_id, uint8_t &ack)
{
	this->pprz_put_byte(&ac_id);
	this->pprz_put_byte((uint8_t) FORWARD_MSG_ID_quad_swarm_ack_forwarded);
	this->pprz_put_byte(&ac_id);
	this->pprz_put_byte(&ack);
}

/*********************************************/
/*  member functions to read pprz message    */
/*********************************************/
uint8_t pprz_msg::pprz_get_msg_id()
{
	if(_length > 2)
	{
		return *(_data_ptr + 1);
	}
	else
		return 0;
}

void pprz_msg::pprz_get_DL_VALUE(uint8_t &ac_id, uint8_t &index, float &value)
{
	ac_id = this->pprz_read_byte();
	this->pprz_read_byte();
	index = this->pprz_read_byte();
	value = this->pprz_read_float();
	return ;
}
//
//  Get the ROTORCRAFT_STATUS
//
void pprz_msg::pprz_get_ROTORCRAFT_STATUS(struct ROTORCRAFT_STATUS &rotorcraft_status)
{
	rotorcraft_status.ac_id = this->pprz_read_byte();
	//skip the msg_id
	this->pprz_read_byte();
	rotorcraft_status.link_imu_nb_err = this->pprz_read_4bytes();
	rotorcraft_status.blmc_nb_err = this->pprz_read_byte();
	rotorcraft_status.rc_status = this->pprz_read_byte();
	rotorcraft_status.frame_rate = this->pprz_read_byte();
	rotorcraft_status.gps_status = this->pprz_read_byte();
	rotorcraft_status.ap_mode = this->pprz_read_byte();
	rotorcraft_status.ap_inflight = this->pprz_read_byte();
	rotorcraft_status.ap_motors_on = this->pprz_read_byte();
	rotorcraft_status.ap_h_mode = this->pprz_read_byte();
	rotorcraft_status.ap_v_mode = this->pprz_read_byte();
	rotorcraft_status.vsupply = this->pprz_read_byte();
}


void pprz_msg::pprz_get_ROTORCRAFT_NAV_STATUS(struct ROTORCRAFT_NAV_STATUS &rotorcraft_nav_status)
{
	rotorcraft_nav_status.ac_id = this->pprz_read_byte();
	//skip the msg_id
	this->pprz_read_byte();
	rotorcraft_nav_status.block_time = this->pprz_read_2bytes();
	rotorcraft_nav_status.stage_time = this->pprz_read_2bytes();
	rotorcraft_nav_status.cur_block = this->pprz_read_byte();
	rotorcraft_nav_status.cur_stage = this->pprz_read_byte();
	rotorcraft_nav_status.horizontal_mode = this->pprz_read_byte();
}

void pprz_msg::pprz_get_quad_swarm_ack(uint8_t &ac_id, uint8_t &quad_swarm_id, uint8_t &quad_swarm_ack)
{
	ac_id = this->pprz_read_byte();
	this->pprz_read_byte();
	quad_swarm_id = this->pprz_read_byte();
	quad_swarm_ack = this->pprz_read_byte();
}

void pprz_msg::pprz_get_quad_swarm_report(struct quad_swarm_report &report)
{
	report.ac_id = this->pprz_read_byte();
	this->pprz_read_byte();
	report.ac_id = this->pprz_read_byte();
	report.x = this->pprz_read_4bytes();
	report.y = this->pprz_read_4bytes();
	report.z = this->pprz_read_4bytes();
	report.state = this->pprz_read_byte();
	report.ap_mode = this->pprz_read_byte();		
	report.pacc = this->pprz_read_4bytes();
}










