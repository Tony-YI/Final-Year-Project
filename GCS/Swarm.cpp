#include "Swarm.h"

Swarm::Swarm()
{
	XBEE_ADDR_HI = (uint32_t *) malloc(sizeof(uint32_t) * (QUAD_NB + 1));
	XBEE_ADDR_LO = (uint32_t *) malloc(sizeof(uint32_t) * (QUAD_NB + 1));
	XBEE_ADDR_HI[1] = XBEE_END_ADDR_A_HI;
	XBEE_ADDR_LO[1] = XBEE_END_ADDR_A_LO;	
	#if QUAD_NB >= 2
	XBEE_ADDR_HI[2] = XBEE_END_ADDR_B_HI;
	XBEE_ADDR_LO[2] = XBEE_END_ADDR_B_LO;
	#endif

	#if QUAD_NB >= 3
	XBEE_ADDR_HI[3] = XBEE_END_ADDR_C_HI;
	XBEE_ADDR_LO[3] = XBEE_END_ADDR_C_LO;
	#endif

	#if QUAD_NB >= 4
	XBEE_ADDR_HI[4] = XBEE_END_ADDR_D_HI;
	XBEE_ADDR_LO[4] = XBEE_END_ADDR_D_LO;
	#endif

	uint8_t count = 1;
	while(count < QUAD_NB + 1)
	{
		state[count] = SWARM_KILLED;
		memset((void *) &pos[count],0,sizeof(struct EcefCoor_i));
		printf("XBEE_ADDR_HI[%d] %x XBEE_ADDR_LO[%d] %x\n",count,XBEE_ADDR_HI[count],count,XBEE_ADDR_LO[count]);
		count++;
	}		
}

Swarm::~Swarm()
{
	free(XBEE_ADDR_HI);
	free(XBEE_ADDR_LO);	
}
uint32_t Swarm::get_address_HI(uint8_t &AC_ID)
{return XBEE_ADDR_HI[AC_ID];}

uint32_t Swarm::get_address_LO(uint8_t &AC_ID)
{return XBEE_ADDR_LO[AC_ID];}

uint8_t Swarm::get_state(uint8_t &AC_ID)
{return state[AC_ID];}

struct EcefCoor_i Swarm::get_quad_coor(uint8_t &AC_ID)
{
	EcefCoor_i result;
	result.x = this->pos[AC_ID].x;
	result.y = this->pos[AC_ID].y;
	result.z = this->pos[AC_ID].z;
	return result;
}

int32_t Swarm::get_pacc(uint8_t &AC_ID)
{return this->pacc[AC_ID];}

void Swarm::set_quad_state(uint8_t &AC_ID, uint8_t s)
{
	
	if(timestamp[AC_ID] < MAX_UINT64_T)
		timestamp[AC_ID]++;
	else
		timestamp[AC_ID] = 0;

	state[AC_ID] = s;
	return ;
}

uint64_t Swarm::get_timestamp(uint8_t &AC_ID)
{
	return timestamp[AC_ID];
}
void Swarm::set_quad_ecef(uint8_t &AC_ID, struct EcefCoor_i &pos)
{
	this->pos[AC_ID].x = pos.x;
	this->pos[AC_ID].y = pos.y;
	this->pos[AC_ID].z = pos.z;
}

void Swarm::set_quad_pacc(uint8_t &AC_ID, int32_t pacc)
{
	this->pacc[AC_ID] = pacc;
}
bool Swarm::all_in_state(uint8_t s)
{
	uint8_t count = 0;
	for(count = 1; count < QUAD_NB + 1;count++)
	{
		if(this->state[count] != s)
		{
			//printf("quad %d  not in state %d but in %d\n",count,s,this->state[count]);
			return false;
		}
	}
	return true;
}
