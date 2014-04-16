#include "main.h"
#include "pprz_msg.h"
#include "XBEE_msg.h"

int main()
{
	uint8_t sender_id = 1;
	uint8_t msg_id = 90;
	uint32_t pos_x = 10;
	uint32_t pos_y = 100;
	uint32_t pos_z = 88;
	pprz_msg data;
	
	uint8_t ac_id = 162;
	uint8_t block_id = 2;
	//data.pprz_set_block(ac_id,block_id);
	uint8_t index = 100;
	float test = 1.1;
	uint32_t num = 16909060;
	data.pprz_set_DL_SETTING(ac_id,index,test);
	data.pprz_put_4bytes((uint8_t *) &num);
	data.show_hex();
	//XBEE_msg xbee_msg;
	//xbee_msg.set_tran_packet(0x00000000,0x0000ffff,0xff,0xfe,data.pprz_get_data_ptr(),data.pprz_get_length());
	//xbee_msg.set_pprz_msg(data);
	//printf("data in xbee_msg\n");
	//xbee_msg.show_hex();
	//xbee_msg.get_pprz_msg().show_hex();
	//pprz_msg recv_data = xbee_msg.get_pprz_msg();
	data.pprz_reset_msg();
	data.show_hex();
	ac_id = data.pprz_read_byte();
	msg_id = data.pprz_read_byte();	
	data.pprz_read_byte();
	index = data.pprz_read_byte();
	float tmp = data.pprz_read_float();
	uint32_t integer = data.pprz_read_4bytes();
	printf("ac_id is %d\n",ac_id);
	printf("msg_id is %d\n",msg_id);
	printf("index is %d\n",index);
	printf("tmp is %f\n",tmp);
	printf("integer %d\n",integer);


	return 0;
}
