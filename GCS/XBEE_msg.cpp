/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/

/*
	THIS FILE IS THE IMPLEMENTATION OF XBEE_msg:: CLASS
*/
#include "XBEE_msg.h"

XBEE_msg::XBEE_msg()
{
	_errorcode = 0;
	_length_HI = 0;
	_length_LO = 0;
	_CheckSum = 0;
	_recv_CheckSum = 0;
	_frameptr = new uint8_t[FRAME_MAX_SIZE];
	frame_length = 0;		
}

XBEE_msg::~XBEE_msg()
{
	#if _DEBUG
	printf("~XBEE_msg(): pointer is %p\n",_frameptr);
	#endif
	//if(_frameptr != NULL)
	//delete _frameptr;
	delete _frameptr;
}

/*
void XBEE_msg::set_frameptr(uint8_t *ptr)
{_frameptr = ptr;}
*/

uint8_t * XBEE_msg::get_frameptr()
{return _frameptr;}

//TODO implement this checksum algorithm
//Calculating the Checksum of an API Packet
//http://www.digi.com/support/kbase/kbaseresultdetl?id=2206
void XBEE_msg::set_CheckSum()
{
	uint32_t sum = 0;
	uint8_t ck = 0;
	uint16_t count = 0;
	for(count = 0;count <= frame_length;count++)
	{
		sum+=*(_frameptr + count);
		#if _DEBUG_CHECKSUM
		printf("At count [%d], add [%x] sum is [%x], recv_ck is [%x]\n",count,*(_frameptr + count),sum,_recv_CheckSum);
		#endif
	}
	//make ck the LSB of the sum
	ck = (sum << 24) >> 24;
	//subtract ck from 0xff to get the final checksum 
	ck = 0xff - ck;
	//printf("Checksum would be [%x]",ck);
	_CheckSum = ck;
}

//THIS FUNCTION IS FOR RECEIVED PACKET TO VERIFY THE CHECKSUM
bool XBEE_msg::verify_CheckSum()
{
	//printf("Verifing CheckSum\n");
	uint32_t sum = 0;
	uint16_t count = 0;
	for(count = 0;count <= frame_length;count++)
	{sum+=*(_frameptr + count);}
	sum+=_recv_CheckSum;
	uint8_t lsb = (sum << 8 * (sizeof(uint32_t) - sizeof(uint8_t))) >> 8 * (sizeof(uint32_t) - sizeof(uint8_t));
	#if _DEBUG_VERIFY_CHECKSUM
	this->set_CheckSum();
	printf("get_CheckSum() return %x\n",this->get_CheckSum());
	if(lsb != 0xff)
	printf("Error: CheckSum received incorrect! lsb [%x]\n",lsb);
	else if(lsb == 0xff)
	printf("CheckSum Correct\n");
	#endif
	return (lsb == 0xff);	
}
uint8_t XBEE_msg::get_CheckSum()
{return _CheckSum;}

void XBEE_msg::set_errorcode(uint8_t errorcode)
{_errorcode = errorcode;}

void XBEE_msg::set_length_LO(uint8_t length_LO)
{_length_LO = length_LO;}

void XBEE_msg::set_length_HI(uint8_t length_HI)
{_length_HI = length_HI;}

void XBEE_msg::set_API_type(uint8_t API_type)
{*(_frameptr) = API_type;}

void XBEE_msg::set_recv_CheckSum(uint8_t recv_CheckSum)
{_recv_CheckSum = recv_CheckSum;}

void XBEE_msg::show_hex()
{
	printf("%x ", START_BYTE);
	printf("%x ", _length_HI);
	printf("%x ", _length_LO);
	uint16_t count = 0;
	while(count < frame_length)
		printf("%x ",*(_frameptr + count++));
	printf("%x %x",_CheckSum,_recv_CheckSum);
	printf("\n");	

}

//frame length is the length between the length and the checksum
void XBEE_msg::set_frame_length()
{frame_length = (_length_HI << 8) + _length_LO;}

uint16_t XBEE_msg::get_frame_length()
{return frame_length;}

uint8_t* XBEE_msg::get_recv_packet_data_ptr()
{return (_frameptr + FRAME_RECV_DATA_OFFSET);}

uint16_t XBEE_msg::get_recv_packet_data_length()
{return (frame_length - FRAME_RECV_DATA_OFFSET + 1);}

uint64_t XBEE_msg::get_recv_source_addr()
{
	uint32_t temp_hi = 0;
	uint32_t temp_lo = 0;
	temp_hi = *(_frameptr + FRAME_RECV_ADDR_OFFSET) << 24;
	temp_hi += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 1) << 16;
	temp_hi += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 2) << 8;
	temp_hi += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 3);
	
	temp_lo = *(_frameptr + FRAME_RECV_ADDR_OFFSET + 4) << 24;
	temp_lo += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 5) << 16;
	temp_lo += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 6) << 8;
	temp_lo += *(_frameptr + FRAME_RECV_ADDR_OFFSET + 7);
	//printf("hi [%08x],lo[%08x]\n",temp_hi,temp_lo);
	uint64_t temp = 0;
 	memcpy(&temp,&temp_hi,sizeof(uint32_t));
	memcpy((uint8_t *)&temp + sizeof(uint32_t),&temp_lo,sizeof(uint32_t));
	return 	temp;
}

uint16_t XBEE_msg::get_recv_source_net_addr()
{return ((*(_frameptr + FRAME_RECV_NET_ADDR_OFFSET)) << sizeof(uint8_t)) + (*(_frameptr + FRAME_RECV_NET_ADDR_OFFSET + sizeof(uint8_t)));}

uint8_t XBEE_msg::get_recv_option()
{return *(_frameptr + FRAME_RECV_OPTION_OFFSET);}

void XBEE_msg::detect_esc()
{frame_length--;}

uint32_t XBEE_msg::get_length_HI()
{return _length_HI;}

uint32_t XBEE_msg::get_length_LO()
{return _length_LO;}

void XBEE_msg::set_tran_dest_addr_HI(const uint32_t &ADDR_HI)
{
	//for little endian
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET) = (ADDR_HI << 0) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 1) = (ADDR_HI << 8) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 2) = (ADDR_HI << 16) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 3) = (ADDR_HI << 24) >> 24;
}
	
void XBEE_msg::set_tran_dest_addr_LO(const uint32_t &ADDR_LO)
{
	//for little endian
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 4) = (ADDR_LO << 0) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 5) = (ADDR_LO << 8) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 6) = (ADDR_LO << 16) >> 24;
	*(_frameptr + FRAME_TRAN_ADDR_OFFSET + 7) = (ADDR_LO << 24) >> 24;
}

void XBEE_msg::set_tran_net_dest_addr_HI(const uint8_t &NET_ADDR_HI)
{*(_frameptr + FRAME_TRAN_NET_ADDR_OFFSET) = NET_ADDR_HI;}

void XBEE_msg::set_tran_net_dest_addr_LO(const uint8_t &NET_ADDR_LO)
{*(_frameptr + FRAME_TRAN_NET_ADDR_OFFSET + 1) = NET_ADDR_LO;}
void XBEE_msg::set_tran_frame_id(const uint8_t &frame_id)
{*(_frameptr + FRAME_TRAN_ID_OFFSET) = frame_id;}

void XBEE_msg::set_tran_data(const uint8_t *data, const uint16_t &size)
{memcpy(_frameptr + FRAME_TRAN_DATA_OFFSET,data,size);}

void XBEE_msg::set_tran_radius(const uint8_t &radius)
{*(_frameptr + FRAME_TRAN_RADIUS_OFFSET) = radius;}

void XBEE_msg::set_tran_option(const uint8_t &option)
{*(_frameptr + FRAME_TRAN_OPTION_OFFSET) = option;}

void XBEE_msg::set_tran_packet(const uint32_t &ADDR_HI,const uint32_t &ADDR_LO,const uint8_t &NET_ADDR_HI, const uint8_t &NET_ADDR_LO,const uint8_t *data, const uint16_t &size)
{
	*(_frameptr) = 0x10;
	this->set_tran_frame_id(0);
	this->set_tran_dest_addr_HI(ADDR_HI);
	this->set_tran_dest_addr_LO(ADDR_LO);
	this->set_tran_net_dest_addr_HI(NET_ADDR_HI);
	this->set_tran_net_dest_addr_LO(NET_ADDR_LO);
	this->set_tran_radius(0);
	this->set_tran_option(0);
	this->set_tran_data(data,size);
	//this->set_CheckSum();
	uint16_t length = FRAME_TRAN_DATA_OFFSET + size;
	_length_HI = length >> 8;
	_length_LO = (length << 8 )>> 8;
	this->set_frame_length();
	this->set_CheckSum();
	//printf("get_CheckSum() %x\n",this->get_CheckSum());
	
}

pprz_msg XBEE_msg::get_pprz_msg()
{	
	pprz_msg pprz_msg1;
	uint16_t count = 0;
	while(count < this->frame_length - FRAME_RECV_DATA_OFFSET)
	{
		pprz_msg1.pprz_put_byte(this->_frameptr + FRAME_RECV_DATA_OFFSET + count++);
	}	
	return pprz_msg1;
}

/*
void XBEE_msg::set_pprz_msg(pprz_msg &pprz_message)
{
	this->set_tran_data(pprz_message.pprz_get_data_ptr(),pprz_message.pprz_get_length());	
	this->set_frame_length();
	this->set_CheckSum();
	return ;
}
*/















