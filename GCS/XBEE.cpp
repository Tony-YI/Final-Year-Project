/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/

//
//	IMPLEMENTATION OF XBEE.h
//
#include "XBEE.h"


//CONSTRUCTOR
XBEE::XBEE(int fd,uint32_t Serial_Num_HI, uint32_t Serial_Num_LO,  uint8_t ID)
{
	this->fd = fd;
	this->Serial_Num_HI = Serial_Num_HI;
	this->Serial_Num_LO = Serial_Num_LO;
	this->ID = ID;
	this->recv_pos = 0;
	this->tran_pos = 0;
	while(!(recv_buff = (uint8_t *) malloc(XBEE_BUFF_SIZE * sizeof(uint8_t))));
	while(!(tran_buff = (uint8_t *) malloc(XBEE_BUFF_SIZE * sizeof(uint8_t))));
	memset(recv_buff,0,XBEE_BUFF_SIZE);
	memset(tran_buff,0,XBEE_BUFF_SIZE);
}

//DESTRUCTOR
XBEE::~XBEE()
{
	//for integrity
	free(recv_buff);
	free(tran_buff);
	close(fd);
}
//TODO possible software flow control
void XBEE::XBEE_write(uint8_t *tran_buff,uint8_t size)
{
	uint16_t byte_count = 0;
	while(byte_count < size)
	{
		byte_count += my_write(this->fd,(void *) (tran_buff + byte_count),size - byte_count);
		printf("byte_count %d\n",byte_count);
	}
	//printf("written\n");
	memset(tran_buff,0,byte_count);
	tran_pos = 0; 
}
//TODO possible software flow control
uint32_t XBEE::XBEE_read(uint8_t * rece_buff,uint32_t size)
{
	return read(this->fd,(void *)rece_buff,size);
}

void XBEE::XBEE_transmit(uint8_t ID,uint8_t * tran_buff, uint8_t size)
{
    this->XBEE_write((uint8_t *)&ID,1);
    this->XBEE_write(tran_buff,size);
}

void XBEE::XBEE_receive(uint8_t * recv_buff, uint8_t size)
{

}

void XBEE::XBEE_set_baud(uint16_t baudrate)
{
	this->baudrate = baudrate;
	return ;
}

void XBEE::XBEE_read_into_recv_buff()
{
	uint32_t byte_count = this->XBEE_read(this->recv_buff + this->recv_pos,XBEE_BUFF_SIZE/2);
	this->recv_pos+=byte_count;
}

/*
	This function parse the data currently in the recv buff into message
*/
void XBEE::XBEE_parse_XBEE_msg()
{
	//current is used to indicate the current position inside recv_buff
	//offset_begin is used int indicate the current starting point of a message
	uint32_t current = 0;// offset_begin = 0;

	//frame_count is used to indicate the current position inside frame
	uint16_t frame_count = 0;
	uint8_t state = 0;
	XBEE_msg * msg_p = NULL;
	//uint8_t *framedata = NULL;
	while(current < recv_pos)
	{
		switch(state)
		{
			case 0: //not yet detect a message
			{
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case0, current %d, recv_pos %d\n",current,recv_pos);
				#endif
				if(recv_buff[current] == START_BYTE)
				{
					if(msg_p == NULL)
					{
					//detect a new message
//					offset_begin = current;
					msg_p = new XBEE_msg();
					#if _DEBUG_XBEE_parse_XBEE_msg
					printf("XBEE_parse_XBEE_msg(): new pointer %p\n",msg_p);
					#endif
					msg.push(msg_p);
					//enter next state if a message is detected
					state++;
					}
					else
					{
						//0x7E inside a message
						//TODO: define error code
						msg_p->set_errorcode(0);
						//msg.push(msg_p);
						msg_p = NULL;
					}
				}
				current++;
				while(current > recv_pos)
					XBEE_read_into_recv_buff();
				break;
			}
			case 1://reading length_HI
			{
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case1\n");
				#endif
				//handle escape character
				#if _USE_XBEE_ESC
				if(recv_buff[current] == 0x7d)
				{
					while(current + 1 > recv_pos)
					XBEE_read_into_recv_buff();
					uint8_t data = recv_buff[++current];
					data = data ^ XOR_BYTE;
					msg_p->set_length_HI(data);
					current++;
				}
				else
				#endif
				{	//if not escape character
					msg_p->set_length_HI(recv_buff[current++]);
				}
				//enter next state to read length_LO
				while(current >= this->recv_pos)
					XBEE_read_into_recv_buff();
				state++;
				break;
			}	
			case 2://reading length_LO
			{
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case2\n");
				#endif
				
				#if _USE_XBEE_ESC
				if(recv_buff[current] == 0x7d)
				{//if escape character
					while(current + 1 > recv_pos)
					XBEE_read_into_recv_buff();
					uint8_t data = recv_buff[++current];
					data = data ^ XOR_BYTE;
					msg_p->set_length_LO(data);
					current++;
				}
				else
				#endif
				{//if not escape character
					msg_p->set_length_LO(recv_buff[current++]);
				}
				msg_p->set_frame_length();
				while(current >= this->recv_pos)
					this->XBEE_read_into_recv_buff();
				//enter next state
				state++;
				break;
			}
			case 3://FRAME TYPE
			{	

				//allocate memory for frame data storage
				//uint8_t *temp =new uint8_t[msg_p->get_frame_length() + 1];
				//msg_p->set_frameptr(temp);
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case3\n");
				printf("msg_p->get_frame_length() %d\n",msg_p->get_frame_length());
				printf("frameptr: [%p]\n",msg_p->get_frameptr());
				#endif
			
				#if _USE_XBEE_ESC
				if(recv_buff[current] == 0x7d)
				{	//if escape character
					while(current + 1 > recv_pos)
					XBEE_read_into_recv_buff();
					uint8_t data = recv_buff[++current];
					//printf("detect escape, original is [%x], changed into",data);
					data = data ^ XOR_BYTE;
					//printf(" [%x]\n",data);
					msg_p->set_API_type(data);
					current++;
				}
				else
				#endif
				{	//if not escape character
					msg_p->set_API_type(recv_buff[current++]);
				}
				frame_count++;
				while(current >= this->recv_pos)
					this->XBEE_read_into_recv_buff();
				state++;
				break;
			}
			case 4://FRAME DATA
			{
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case4\n");
				#endif
				uint8_t *ptr = msg_p->get_frameptr();
				//TODO: POSSIBLE BUGS HERE
				while(frame_count < msg_p->get_frame_length() && current < recv_pos)
				{
					#if _USE_XBEE_ESC
					if(recv_buff[current] == 0x7d)
					{	//if escape character
						msg_p->detect_esc();
						while(current + 1 > recv_pos)
							XBEE_read_into_recv_buff();
						uint8_t data = recv_buff[++current];
						//printf("detect escape, original is [%x], changed into",data);
						data = data ^ XOR_BYTE;
						//printf(" [%x]\n",data);
						#if _DEBUG_XBEE_parse_XBEE_msg
						printf("loading %x into %d [%p],framelength %d\n",data,frame_count,ptr + frame_count,msg_p->get_frame_length());
						#endif
						*(ptr + frame_count++) = data;
						current++;
					}
					else
					#endif
					{
						#if _DEBUG_XBEE_parse_XBEE_msg
						printf("loading %x into %d [%p],framelength %d\n",recv_buff[current],frame_count,ptr+frame_count,msg_p->get_frame_length());
						#endif
						*(ptr + frame_count++) = recv_buff[current++];	
					}
					
					if(current >= recv_pos)
					{
						//TODO: handle the case that the framedata is not completely read
						//into the recv_buff
						//fprintf(stderr,"XBEE_ERROR: the framedata is not fully read\n");
						while(current >= this->recv_pos)
							this->XBEE_read_into_recv_buff();
						//continue;					
						//return ;
					}
				}
				state++;
				break;
			}
			case 5://CheckSum
			{
				#if _DEBUG_XBEE_parse_XBEE_msg
				printf("case5, checksum would be [%x]\n",recv_buff[current]);
				printf("framelength is %d\n",msg_p->get_frame_length());
				#endif
				
				#if _USE_XBEE_ESC
				if(recv_buff[current] == 0x7D)
				{
					//if escape character
					while(current + 1 > recv_pos)
						XBEE_read_into_recv_buff();
					uint8_t data = recv_buff[++current];
					data = data ^ XOR_BYTE;
					msg_p->set_recv_CheckSum(data);
				}
				else
				#endif
				
				{msg_p->set_recv_CheckSum(recv_buff[current++]);}	
				//Reset all parameters and ready to read the next message
				state = 0;
				#if _DEBUG_CHECKSUM
				if(!msg_p->verify_CheckSum())
				printf("Checksum error\n");
				else
				printf("CheckSum Correct\n");
				#endif
				//msg_p->set_CheckSum();
				msg_p = NULL;
				frame_count = 0;
				break;	
			}
			default:
			{
				fprintf(stderr,"XBEE_ERROR: unexpeced error\n");
				return ;
			}
		}	
	}
	//if(state == 2)
	if(recv_pos + 1 >= XBEE_BUFF_SIZE)
		memset(recv_buff,0,XBEE_BUFF_SIZE);
	else
		memset(recv_buff,0,recv_pos + 1);
	recv_pos = 0;
}

void XBEE::XBEE_show_msg()
{
	XBEE_msg * ptr = NULL;
	while(!msg.empty())
	{
		ptr = msg.front();
		
		msg.pop();
		ptr->show_hex();
		delete ptr;
	}
}

void XBEE::XBEE_show_recv_buff()
{
	uint32_t count = 0;
	printf("XBEE->recv_buff:\n");
	while(count < recv_pos)
	{
		printf("%x ", *(recv_buff + count++));
	}
	printf("\n");
}

/*
TODO handle escape character.
TODO increase the length record in the tran_buff when a escape character is detected
TODO when writing the frame data into the tran_buff, also remember the couting of data written into the tran_buff should be affected (not simply count++)	
*/	
void XBEE::XBEE_send_msg(XBEE_msg &msg)
{
	uint8_t state = 0;
	uint8_t data = 0;
	while(state < 5)
	{
		switch(state)
		{
		case (0):
			{
			tran_buff[tran_pos++] = START_BYTE;
			state++;
			break;
			}
		case(1):
			{
			data = msg.get_length_HI();
			#if _USE_XBEE_ESC	
			if(data == START_BYTE || data == ESC_BYTE || data == XON_BYTE || data == XOFF_BYTE)
			{//handle escape character
				data = data ^ XOR_BYTE;
				tran_buff[tran_pos++] = ESC_BYTE;	
			}
			#endif
			tran_buff[tran_pos++] = data;
			state++;
			break;
			}
		case(2):
			{
			tran_buff[tran_pos++] = msg.get_length_LO();
			state++;
			break;
			}
		case(3):
			{
			uint8_t *ptr = msg.get_frameptr();
			uint16_t length = msg.get_frame_length();
			uint16_t count = 0;
			while(count < length)
			{
				#if _DEBUG_XBEE_SEND_MSG
                                printf("writing [%x]  into %d in tran_buff, count is [%d] length [%d]\n",*(ptr + count),tran_pos,count,length);
                                #endif
				tran_buff[tran_pos++] = *(ptr+count++);
			}
			state++;
			break;
			}
		case(4):
			{
				#if _DEBUG_XBEE_SEND_MSG
				printf("get_CheckSum() return %d\n",msg.get_CheckSum());
				#endif
				tran_buff[tran_pos++] = msg.get_CheckSum();
				state++;	
				break;
			}
		default:
			{
			printf("error in XBEE_send_msg()\n");
			}		
		}
		#if _DEBUG_XBEE_SEND_MSG
		printf("state %d, [%x] written into %d in tran_buff\n",state,tran_buff[tran_pos - 1],tran_pos - 1);
		#endif	
	}
	XBEE_write(this->tran_buff,tran_pos);

	//clean the buffer content	
	if(tran_pos + 1 >= XBEE_BUFF_SIZE)
		memset(tran_buff,0,XBEE_BUFF_SIZE);
	else
		memset(tran_buff,0,tran_pos + 1);	
	tran_pos = 0;
}









