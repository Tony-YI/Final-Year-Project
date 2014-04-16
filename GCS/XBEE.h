/*

	This source code is written by Edward HUANG, and Tony Yi
	For the development of their Final Year Project
	Department of Computer Science and Engineering
	The Chinese University of HONG KONG

*/
#include "main.h"
#ifndef _XBEE_H
#define _XBEE_H

#if _USE_MBED

#include "mbed.h"

#else
//for interaction with uer
#include <stdio.h>

//for memset() and uint8_t
#include <stdlib.h>
//for read() and write()
#include <unistd.h>
#include <queue>
#include "XBEE_msg.h"
#include "Serial.h"

#endif

//#include "ADDRESS.h"
#define XBEE_BUFF_SIZE 5 * MB

class XBEE
{
    private:
	//The file descriptor for the port
	int fd;
	//the baudrate of the port
        uint16_t baudrate;
	//Serial Number of the currently connected XBEE module
	uint32_t Serial_Num_HI;
	uint32_t Serial_Num_LO;
        //set for future usage
	uint8_t ID;
	//the transmit buff for data going out
	uint8_t	*tran_buff;
	//the receive buff for data coming in
	uint8_t *recv_buff;
	//the record of the current usage of transmit buff
	uint64_t tran_pos;
	//the record of the current usage of the receive buff
	uint64_t recv_pos;
	//the queue stroing pointer to XBEE message parsed from receive buff

	//std::queue<XBEE_msg *> msg;

    public:
	std::queue<XBEE_msg *> msg;
	//Constructor for XBEE module, for data transmission and receving, and some configuration
	//fd is the file descriptor open by fopen(), and should be pre-configured by set_interface_attribs() and set_blocking() provided by Serial.h
	//Serial_Num_HI, Serial_Num_LO and ID are just for record, no usage for now
        XBEE(int fd,uint32_t Serial_Num_HI = 0x00000000,uint32_t Serial_Num_LO = 0x0000ffff, uint8_t ID = 0);

	//Destructor for XBEE module
        ~XBEE();
	
	//This function will definitely write [size] bytes from tran_buff through the port  
        void XBEE_write(uint8_t *tran_buff,uint8_t size);
	//This function will try to read [size] bytes from the port into the recv_buff, but the number is not guarenteed and the actual number of bytes read will be returned. 
        uint32_t XBEE_read(uint8_t * rece_buff,uint32_t size);

	//This function is not for usage now
        void XBEE_transmit(uint8_t ID,uint8_t * tran_buff, uint8_t size);
        //This function is not for usage now
	void XBEE_receive(uint8_t * rece_buff,uint8_t size);
	
	//This function record the baudrate of the port. NOTE that you should use set_interface_attribs() from Serial.h to really configure the baudrate
        void XBEE_set_baud(uint16_t baudrate);
	
	//This function will try to read [XBEE_BUFF_SIZE/2] bytes into the recv_buff
	void XBEE_read_into_recv_buff();
	
	//This function will try parse the current data inside recv_buff, and construct XBEE_msg objects and store them into queue<XBEE_msg *> msg variable 
	void XBEE_parse_XBEE_msg();
	
	//This function will show and pop out all the message inside the msg, 
	void XBEE_show_msg();
	
	//This function will show all the data inside the recv_buff, used for debuging
	void XBEE_show_recv_buff();
	//This function is used for sending out a well configured XBEE_msg object through the port	
	void XBEE_send_msg(XBEE_msg &msg);
};
#endif
