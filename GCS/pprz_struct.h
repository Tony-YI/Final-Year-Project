#ifndef _PPRZ_STRUCT_H
#define _PPRZ_STRUCT_H

#include "main.h"
struct ROTORCRAFT_STATUS
{
	uint8_t ac_id;
	uint32_t link_imu_nb_err;
	uint8_t blmc_nb_err;
	uint8_t rc_status;
	uint8_t frame_rate;
	uint8_t gps_status;
	uint8_t ap_mode;
	uint8_t ap_inflight;
	uint8_t ap_motors_on;
	uint8_t ap_h_mode;
	uint8_t ap_v_mode;
	uint8_t vsupply;
};

struct ROTORCRAFT_NAV_STATUS
{
	uint8_t ac_id;
	uint16_t block_time;
	uint16_t stage_time;
	uint8_t cur_block;
	uint8_t cur_stage;
	uint8_t horizontal_mode;
};

struct quad_swarm_msg
{
	uint8_t ac_id;
	uint8_t dummy;
	int32_t x;
	int32_t y;
	int32_t z;
};

struct quad_swarm_report
{
	uint8_t ac_id;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint8_t state;
	uint8_t ap_mode;
	int32_t pacc;
};
#endif
