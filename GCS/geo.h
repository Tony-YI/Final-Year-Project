/*

        This source code is written by Edward HUANG, and Tony Yi
        For the development of their Final Year Project
        Department of Computer Science and Engineering
        The Chinese University of HONG KONG

*/

/* 
		This source code mostly copy the implemenetation of
		geo functions in the paparazzi open source project
		paparazzi website: http://wiki.paparazziuav.org/wiki/Main_Page
*/

#ifndef _GEO_H
#define _GEO_H
#include "main.h"
#include <cmath>
#include "GCS_math.h"

//-----------------------------------------------//
//-Struct definition for geographical function---//
//-----------------------------------------------//

/***************************************/
/*             ECEF STRUCT             */
/***************************************/
struct EcefCoor_i
{
  int32_t x;
  int32_t y;
  int32_t z;
};

struct EcefCoor_d {
  double x; ///< in meters
  double y; ///< in meters
  double z; ///< in meters
};

/**************************************/
/*             LLA STRUCT             */
/**************************************/       
struct LlaCoor_i
{
	//in radians *1e7
  int32_t lon;
  int32_t lat;
  int32_t alt;
};
struct LlaCoor_d {
  double lon; ///< in radians
  double lat; ///< in radians
  double alt; ///< in meters above WGS84 reference ellipsoid
};

///////////////////
struct NedCoor_i
{
	//north
	int32_t x;
	//east
	int32_t y;
	//down
	int32_t z;
};
//////////////////
struct EnuCoor_i
{
	int32_t x;
	int32_t y;
	int32_t z;
};
//////////////////
struct LtpDef_i
{
	//reference point in ecef coordinate system
	struct EcefCoor_i ecef;
	//reference point in lla coordinate system
	struct LlaCoor_i lla;
	//Rotational matrix of ecef w.r.t local tangent plane
	struct Int32Mat33 ltp_of_ecef;
	//height above mean sea level
	int32_t hmsl;
};

//-----------------------------------------------------//
//-Function prototypes for geographical functionality--//
//-----------------------------------------------------//


//*****************************************************//
//*This function takes 'ecef' as the input, and compute//
//*the 'ned' as the output, w.r.t 'def', the output ned//
//*is in unit centimeter                              *//
//*****************************************************//

/* In ins_int.c it is implemented as 
void ins_update_gps(void) {
#if USE_GPS
	if (gps.fix == GPS_FIX_3D)
	{
		if (!ins_ltp_initialised)
		{	//initialised the INS local tangent plane coordinate system	
			ltp_def_from_ecef_i(&ins_ltp_def, &gps.ecef_pos);
			ins_ltp_def.lla.alt = gps.lla_pos.alt;
			ins_ltp_def.hmsl = gps.hmsl;
			ins_ltp_initialised = TRUE;
			stateSetLocalOrigin_i(&ins_ltp_def);
		}
		//the output of ned_of_ecef_point_i is in NedCoor_i, and it's in unit of centimeter
		ned_of_ecef_point_i(&ins_gps_pos_cm_ned, &ins_ltp_def, &gps.ecef_pos);
		ned_of_ecef_vect_i(&ins_gps_speed_cm_s_ned, &ins_ltp_def, &gps.ecef_vel);
		#if USE_HFF // if horizontal filter is used 
		VECT2_ASSIGN(ins_gps_pos_m_ned, ins_gps_pos_cm_ned.x, ins_gps_pos_cm_ned.y);
		VECT2_SDIV(ins_gps_pos_m_ned, ins_gps_pos_m_ned, 100.);
		VECT2_ASSIGN(ins_gps_speed_m_s_ned, ins_gps_speed_cm_s_ned.x, ins_gps_speed_cm_s_ned.y);
		VECT2_SDIV(ins_gps_speed_m_s_ned, ins_gps_speed_m_s_ned, 100.);
		if(ins.hf_realign)
		{
			ins.hf_realign = FALSE;
			#ifdef SITL
			struct FloatVect2 true_pos, true_speed;
			VECT2_COPY(true_pos, fdm.ltpprz_pos);
			VECT2_COPY(true_speed, fdm.ltpprz_ecef_vel);
			b2_hff_realign(true_pos, true_speed);
			#else
			const struct FloatVect2 zero = {0.0, 0.0};
			b2_hff_realign(ins_gps_pos_m_ned, zero);
			#endif
		}
		b2_hff_update_gps();

		#if !USE_VFF // vff not used 
		ins_ltp_pos.z = (ins_gps_pos_cm_ned.z * INT32_POS_OF_CM_NUM) / INT32_POS_OF_CM_DEN;
		ins_ltp_speed.z = (ins_gps_speed_cm_s_ned.z * INT32_SPEED_OF_CM_S_NUM) INT32_SPEED_OF_CM_S_DEN;
		#endif // vff not used 
		#endif // hff used  
		#if !USE_HFF // hff not used //
		#if !USE_VFF // neither hf nor vf used 
		INT32_VECT3_SCALE_3(ins_ltp_pos, ins_gps_pos_cm_ned, INT32_POS_OF_CM_NUM, INT32_POS_OF_CM_DEN);
		INT32_VECT3_SCALE_3(ins_ltp_speed, ins_gps_speed_cm_s_ned, INT32_SPEED_OF_CM_S_NUM, INT32_SPEED_OF_CM_S_DEN);
		#else // only vff used 
		INT32_VECT2_SCALE_2(ins_ltp_pos, ins_gps_pos_cm_ned, INT32_POS_OF_CM_NUM, INT32_POS_OF_CM_DEN);
		INT32_VECT2_SCALE_2(ins_ltp_speed, ins_gps_speed_cm_s_ned, INT32_SPEED_OF_CM_S_NUM, INT32_SPEED_OF_CM_S_DEN);
		#endif
		#if USE_GPS_LAG_HACK
		VECT2_COPY(d_pos, ins_ltp_speed);
		INT32_VECT2_RSHIFT(d_pos, d_pos, 11);
		VECT2_ADD(ins_ltp_pos, d_pos);
		#endif
		#endif // hff not used 
		INS_NED_TO_STATE();
	}
#endif // USE_GPS 
}

*/

/*********************************************************/
/*        Convert coordiate from ECEF to others          */
/*********************************************************/
extern void ned_of_ecef_point_i(struct NedCoor_i *ned, struct LtpDef_i *def, struct EcefCoor_i *ecef);
extern void enu_of_ecef_point_i(struct EnuCoor_i* enu, struct LtpDef_i* def, struct EcefCoor_i* ecef);

//input LtpDef_i def, EcefCoor_i ecef, output NedCoor_i ned
extern void ned_of_ecef_pos_i(struct NedCoor_i* ned, struct LtpDef_i* def, struct EcefCoor_i* ecef);
//input LtpDef_i def, EcefCoor_i ecef, output EnuCoor_i enu
extern void enu_of_ecef_pos_i(struct EnuCoor_i* enu, struct LtpDef_i* def, struct EcefCoor_i* ecef);

//input EcefCoor_d * ecef, output LlaCoor_d *lla, to be called by lla_of_ecef_i
extern void lla_of_ecef_d(struct LlaCoor_d* lla, struct EcefCoor_d* ecef);

//input EcefCoor_d, output LlaCoor_d
extern void lla_of_ecef_i(struct LlaCoor_i* out, struct EcefCoor_i* in);


//set the LtpDef_i def according to EcefCoor_i
extern void ltp_def_from_ecef_i(struct LtpDef_i* def, struct EcefCoor_i* ecef);


extern void enu_of_ecef_point_i(struct EnuCoor_i *enu, struct LtpDef_i *def, struct EcefCoor_i *ecef);

/**********************************************************/
/*        Convert coordinate from NED to others           */
/**********************************************************/
extern void ecef_of_ned_pos_i(struct EcefCoor_i * ecef,struct LtpDef_i * def, struct NedCoor_i * ned);
extern void ecef_of_ned_point_i(struct EcefCoor_i * ecef,struct LtpDef_i * def, struct NedCoor_i * ned);
extern void ecef_of_enu_pos_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu);
extern void ecef_of_enu_vect_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu);
extern void ecef_of_enu_point_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu);

//-----------------------------------------------------//
//--MACRO for implementation of geopraphical functions-//
//-----------------------------------------------------//

#define ENU_OF_TO_NED(_po, _pi)\
{\
	(_po).x = (_pi).y;\
	(_po).y = (_pi).x;\
	(_po).z = -(_pi).z;\
}


/******************************************************/
/*         MACRO from pprz_algebra_int.h              */
/******************************************************/

#define HIGH_RES_TRIG_FRAC 20

#define BFP_OF_REAL(_vr, _frac) ((_vr)*(1<<(_frac)))

#define FLOAT_OF_BFP(_vbfp,_frac) ((float)(_vbfp)/(1<<(_frac)))

#define INT32_VECT3_RSHIFT(_o, _i, _r) { \
    (_o).x = ((_i).x >> (_r));		 \
    (_o).y = ((_i).y >> (_r));		 \
    (_o).z = ((_i).z >> (_r));		 \
}

#define INT32_VECT3_ADD(_a, _b) VECT3_ADD(_a, _b)

/******************************************************/
/*      vector operation from pprz_algebra.h          */
/******************************************************/
#define VECT3_SDIV(_vo, _vi, _s){\
	(_vo).x = (_vi).x / (_s);\
	(_vo).y = (_vi).y / (_s);\
	(_vo).z = (_vi).z / (_s);\
}

#define VECT3_COPY(_a, _b) {\
    (_a).x = (_b).x;\
    (_a).y = (_b).y;\
    (_a).z = (_b).z;\
}

#define VECT3_SMUL(_vo, _vi, _s) {			\
    (_vo).x =  (_vi).x * (_s);				\
    (_vo).y =  (_vi).y * (_s);				\
    (_vo).z =  (_vi).z * (_s);				\
}

#define VECT3_ADD(_a, _b) {			\
    (_a).x += (_b).x;				\
    (_a).y += (_b).y;				\
    (_a).z += (_b).z;				\
}
//------------------------------------------------------
//------------------------------------------------------

/******************************************************/
/*         MACRO from pprz_geodetic_int.h             */
/******************************************************/
#define POS_FLOAT_OF_BFP(_ai) FLOAT_OF_BFP((_ai),INT32_POS_FRAC)

#define CM_OF_M(_m)  ((_m)*1e2)
#define M_OF_CM(_cm) ((_cm)/1e2)
#define MM_OF_M(_m)  ((_m)*1e3)
#define M_OF_MM(_mm) ((_mm)/1e3)
#define EM7RAD_OF_RAD(_r) ((_r)*1e7)
#define RAD_OF_EM7RAD(_r) ((_r)/1e7)

#endif
