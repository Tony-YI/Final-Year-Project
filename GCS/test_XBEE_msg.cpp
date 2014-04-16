#include "XBEE_msg.h"

int main(void)
{
	for(int count = 0 ;count < 2; count++)
{
	uint8_t ac_id = 0x01, msg_id = RECV_MSG_ID_quad_swarm_report;
	int32_t x = 0, y = 0,z = 0, pacc = 500;
	uint8_t state = 4 + count, ap_mode = 12;  
	pprz_msg data;
	data.pprz_put_byte(ac_id);
	data.pprz_put_byte(msg_id);
	data.pprz_put_byte(ac_id);
	data.pprz_put_4bytes((uint8_t *) &x);
	data.pprz_put_4bytes((uint8_t *) &y);
	data.pprz_put_4bytes((uint8_t *) &z);
	data.pprz_put_byte(state);
	data.pprz_put_byte(ap_mode);
	data.pprz_put_4bytes((uint8_t *) &pacc);
	XBEE_msg tmp;
	tmp.set_tran_packet(0x0013a200,0x40992206,0xff,0xfe,data.pprz_get_data_ptr(),data.pprz_get_length());
	printf("whole message: \n");
	tmp.show_hex();
	printf("message end \n");
}
	return 0;

}
