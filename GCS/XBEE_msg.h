/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/
#ifndef _XBEE_MSG_H
#define _XBEE_MSG_H
#include "main.h"
#include "pprz_msg.h"

#define START_BYTE 0x7E
#define ESC_BYTE 0x7D
#define XON_BYTE 0x11
#define XOFF_BYTE 0x13
#define XOR_BYTE 0x20
/*---------MACRO FOR XBEE receive RF packet----------------
-----------------------------------------------------------
-----------------------------------------------------------*/

//frame type 1 byte, serial number 8 byte, network address 2 byte, option 1 byte 
#define FRAME_RECV_ADDR_OFFSET 1
#define FRAME_RECV_NET_ADDR_OFFSET 9
#define FRAME_RECV_OPTION_OFFSET 11
#define FRAME_RECV_DATA_OFFSET 12 

/*--------MACRO FOR THE XBEE TRANSMIT REQUEST---------------
------------------------------------------------------------
-----------------------------------------------------------*/
#define FRAME_TRAN_ID_OFFSET 1
#define FRAME_TRAN_ADDR_OFFSET 2
#define FRAME_TRAN_NET_ADDR_OFFSET 10
#define FRAME_TRAN_RADIUS_OFFSET 12
#define FRAME_TRAN_OPTION_OFFSET 13
#define FRAME_TRAN_DATA_OFFSET 14

#define XBEE_NET_ADDR_HI 255
#define XBEE_NET_ADDR_LO 254

#define FRAME_MAX_SIZE 512
class XBEE_msg
{
	private:
		//_errorcode is not for used now, preserverd for future
		uint8_t _errorcode;

		//_length_HI and _length_LO are the two bytes following the START_BYTE in a XBEE_API message.
		//They indictaes the length after the third byte of the receive packet and before the CheckSum
		uint8_t _length_HI;
		uint8_t _length_LO;
		//uint8_t _API_type;
		
		//_CheckSum is computed by set_CheckSum() and it's only possibly used for a XBEE_msg that it's configure by you.
		uint8_t _CheckSum;
		//_recv_CheckSum is set when a XBEE_msg is parsed by XBEE::XBEE_parse_XBEE_msg() from XBEE.h, if you want to know whether this _recv_CheckSum is valid, please call verify_CheckSum();
		uint8_t _recv_CheckSum;
	
		//_frameptr is the pointer to the buffer where data after _length_LO, before CheckSum would be stored.
		//Note that the size of buffer is defined by FRAME_MAX_SIZE, and space will be allocated by the constructor
		uint8_t * _frameptr;

		//frame_length is the length computed from _length_HI and _length_LO, and it set for convenience for scaning though the _frameptr.
		uint16_t frame_length;
	public:
		//Constructor for the XBEE_msg();
		//Initialized every thing to 0 or NULL	
		XBEE_msg();

		//Destructor for the XBEE_msg();
		//delete _frameptr will be called here
		~XBEE_msg();
				
		//return the pointer pointing to the framedata
		uint8_t * get_frameptr();
		
		//set_CheckSum() will computed the CheckSum after you have stored data into the _frameptr and called set_frame_length()
		void set_CheckSum();
		//get_CheckSum() will retured the CheckSum() computed locally
		uint8_t get_CheckSum();
		//not for usage now
		void set_errorcode(uint8_t errorcode);
		
		//set_length_HI() and set_length_LO() will set the _length_HI and _length_LO, for further computaion of frame_length
		void set_length_LO(uint8_t length_LO);
		void set_length_HI(uint8_t length_HI);
		//set_API_type() will set _API_type
		//TODO maybe removed this function, or make it directly write into *(_frameptr) 
		void set_API_type(uint8_t API_type);
		
		//set_recv_CheckSum() is called in XBEE::XBEE_parse_XBEE_msg() to set the _recv_CheckSum;
		void set_recv_CheckSum(uint8_t recv_CheckSum);
		//show_hex() will display the hexidecimal data in _frameptr
		void show_hex();
		//not for usage now
		void detect_esc();
		//this function compute frame length according to the _length_HI and _length_LO, remember to call this before you call set_CheckSum()
		void set_frame_length();
		//return the frame_length variable
		uint16_t get_frame_length();
		/*---------------------------------------------------------------
		IF THIS PACKET IS A XBEE RECEIVE PACKET WITH FRAME TYPE AS 0x90
		THEN THE FOLLOWING FUNCTION WOULD BE USEFUL
		---------------------------------------------------------------*/
		//this function directly return the pointer at the first byte of the data embedded by the frame header for a received RF packet
		uint8_t *get_recv_packet_data_ptr();
	
		//this function directly return the length of the data inside the frame for a received RF packet 
		uint16_t get_recv_packet_data_length();
		//this function will return the Serial Number of the senderfor a received RF packet
		uint64_t get_recv_source_addr();
		//this function will return the Network address of the sender for a received RF packet
		uint16_t get_recv_source_net_addr();
		//this function will return the option field of the recved RF packet
		uint8_t get_recv_option();
		//this function will verify the CheckSum value for the received RF packet
		bool verify_CheckSum();


		/*
			The following functions are defined for message
			sending out.
		*/
		//not for usage now
		uint32_t get_length_HI();
		//not for usage now
		uint32_t get_length_LO();

		//this function will initilized the current XBEE_msg object as a transmiting packet
		//with destionation address_HI and destionation address_LO, and with network address_hi, network address_lo, and the pointer to the data you're going to send, and the length of the data
		//CheckSum value are also computed inside this function
		//the frame_id is consider as 0, radius 0 and option 0
		//if you want to change the above value, please manually called set_tran_frame_id(), set_tran_radius() and set_tran_option() and remeber to call set_CheckSum() after all change you have made
		void set_tran_packet(const uint32_t &ADDR_HI,const uint32_t &ADDR_LO,const uint8_t &NET_ADDR_HI, const uint8_t &NET_ADDR_LO,const uint8_t *data, const uint16_t &size);
		
		//for the usage of set_tran_packet
		void set_tran_dest_addr_HI(const uint32_t &ADDR_HI);
		void set_tran_dest_addr_LO(const uint32_t &ADDR_LO);
		void set_tran_net_dest_addr_HI(const uint8_t &NET_ADDR_HI);
		void set_tran_net_dest_addr_LO(const uint8_t &NET_ADDR_LO);
		void set_tran_frame_id(const uint8_t &frame_id);
		void set_tran_radius(const uint8_t &radius);
		void set_tran_option(const uint8_t &option);
		void set_tran_data(const uint8_t *data,const uint16_t &size);
		//------------------------------------------//





		//for the compatible usage of pprz_msg and XBEE_msg
		
		pprz_msg get_pprz_msg();

		//void set_pprz_msg(pprz_msg &pprz_message);	
};



#endif

