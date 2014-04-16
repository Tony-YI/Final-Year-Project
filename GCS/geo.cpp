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

#include "geo.h"
#include "GCS_math.h"

/**************************************************************/
/**************************************************************/
/*             Convert ECEF coordinate to others              */
/**************************************************************/
/**************************************************************/

void ned_of_ecef_point_i(struct NedCoor_i *ned, struct LtpDef_i *def, struct EcefCoor_i *ecef)
{
	struct EnuCoor_i enu;
	enu_of_ecef_point_i(&enu, def, ecef);
	ENU_OF_TO_NED(*ned, enu);
}
void enu_of_ecef_point_i(struct EnuCoor_i* enu, struct LtpDef_i* def, struct EcefCoor_i* ecef) {

  struct EcefCoor_i delta;
  VECT3_DIFF(delta, *ecef, def->ecef);
  //printf("delta: x %d, y %d, z %d\n",delta.x,delta.y,delta.z);
  const int64_t tmpx = (int64_t)def->ltp_of_ecef.m[0]*delta.x +
                       (int64_t)def->ltp_of_ecef.m[1]*delta.y +
                       0; /* this element is always zero http://en.wikipedia.org/wiki/Geodetic_system#From_ECEF_to_ENU */
  //printf("enu_of_ecef_point_i x %d",enu->x);
  enu->x = (int32_t)(tmpx>>HIGH_RES_TRIG_FRAC);
  const int64_t tmpy = (int64_t)def->ltp_of_ecef.m[3]*delta.x +
                       (int64_t)def->ltp_of_ecef.m[4]*delta.y +
                       (int64_t)def->ltp_of_ecef.m[5]*delta.z;
  //printf(" y %d",enu->y);
  enu->y = (int32_t)(tmpy>>HIGH_RES_TRIG_FRAC);
  const int64_t tmpz = (int64_t)def->ltp_of_ecef.m[6]*delta.x +
                       (int64_t)def->ltp_of_ecef.m[7]*delta.y +
                       (int64_t)def->ltp_of_ecef.m[8]*delta.z;
  //printf(" z %d\n",enu->z);
  enu->z = (int32_t)(tmpz>>HIGH_RES_TRIG_FRAC);
  //printf("after conversion: x %d y %d z%d\n",enu->x,enu->y,enu->z);
}
/** Convert a ECEF position to local NED.
 * @param[out] ned  NED position in meter << #INT32_POS_FRAC
 * @param[in]  def  local coordinate system definition
 * @param[in]  ecef ECEF position in cm
 */
void ned_of_ecef_pos_i(struct NedCoor_i* ned, struct LtpDef_i* def, struct EcefCoor_i* ecef) {
  struct EnuCoor_i enu;
  enu_of_ecef_pos_i(&enu, def, ecef);
  //printf("enu x %d, y %d, z %d\n",enu.x,enu.y,enu.z);
  ENU_OF_TO_NED(*ned, enu);
}
/** Convert a ECEF position to local ENU.
 * @param[out] enu  ENU position in meter << #INT32_POS_FRAC
 * @param[in]  def  local coordinate system definition
 * @param[in]  ecef ECEF position in cm
 */
void enu_of_ecef_pos_i(struct EnuCoor_i* enu, struct LtpDef_i* def, struct EcefCoor_i* ecef) {
  struct EnuCoor_i enu_cm;
  enu_of_ecef_point_i(&enu_cm, def, ecef);

  /* enu = (enu_cm / 100) << INT32_POS_FRAC
   * to loose less range:
   * enu_cm = enu << (INT32_POS_FRAC-2) / 25
   * which puts max enu output Q23.8 range to 8388km / 25 = 335km
   */
  INT32_VECT3_LSHIFT(*enu, enu_cm, INT32_POS_FRAC-2);
  VECT3_SDIV(*enu, *enu, 25);
}

//from math/pprz_geodetic_int.c
void lla_of_ecef_i(struct LlaCoor_i* out, struct EcefCoor_i* in) 
{
  /* convert our input to floating point */
  struct EcefCoor_d in_d;
  in_d.x = M_OF_CM((double)in->x);
  in_d.y = M_OF_CM((double)in->y);
  in_d.z = M_OF_CM((double)in->z);
  /* calls the floating point transformation */
  struct LlaCoor_d out_d;
  lla_of_ecef_d(&out_d, &in_d);
  /* convert the output to fixed point       */
  out->lon = (int32_t)rint(EM7RAD_OF_RAD(out_d.lon));
  out->lat = (int32_t)rint(EM7RAD_OF_RAD(out_d.lat));
  out->alt = (int32_t)MM_OF_M(out_d.alt);
}

//from math/pprz_geodetic_double.c, to be called by lla_of_ecef_i
void lla_of_ecef_d(struct LlaCoor_d* lla, struct EcefCoor_d* ecef) {

  // FIXME : make an ellipsoid struct
  static const double a = 6378137.0;           /* earth semimajor axis in meters */
  static const double f = 1./298.257223563;    /* reciprocal flattening          */
  const double b = a*(1.-f);                   /* semi-minor axis                */
  const double b2 = b*b;

  const double e2 = 2.*f-(f*f);                /* first eccentricity squared     */
  const double ep2 = f*(2.-f)/((1.-f)*(1.-f)); /* second eccentricity squared    */
  const double E2 = a*a - b2;


  const double z2 = ecef->z*ecef->z;
  const double r2 = ecef->x*ecef->x+ecef->y*ecef->y;
  const double r = sqrt(r2);
  const double F = 54.*b2*z2;
  const double G = r2 + (1-e2)*z2 - e2*E2;
  const double c = (e2*e2*F*r2)/(G*G*G);
  const double s = pow( (1 + c + sqrt(c*c + 2*c)), 1./3.);
  const double s1 = 1+s+1/s;
  const double P = F/(3*s1*s1*G*G);
  const double Q = sqrt(1+2*e2*e2*P);
  const double ro = -(e2*P*r)/(1+Q) + sqrt((a*a/2)*(1+1/Q) - ((1-e2)*P*z2)/(Q*(1+Q)) - P*r2/2);
  const double tmp = (r - e2*ro)*(r - e2*ro);
  const double U = sqrt( tmp + z2 );
  const double V = sqrt( tmp + (1-e2)*z2 );
  const double zo = (b2*ecef->z)/(a*V);

  lla->alt = U*(1 - b2/(a*V));
  lla->lat = atan((ecef->z + ep2*zo)/r);
  lla->lon = atan2(ecef->y,ecef->x);

}
/*************************************************************************/
/*         The function to set LtpDef_i according to EcefCoor_i          */
/*************************************************************************/
void ltp_def_from_ecef_i(struct LtpDef_i* def, struct EcefCoor_i* ecef) {

  /* store the origin of the tangeant plane */
  VECT3_COPY(def->ecef, *ecef);
  /* compute the lla representation of the origin */
  lla_of_ecef_i(&def->lla, &def->ecef);
  /* store the rotation matrix                    */

#if 1
  int32_t sin_lat = rint(BFP_OF_REAL(sinf(RAD_OF_EM7RAD((float)def->lla.lat)), HIGH_RES_TRIG_FRAC));
  int32_t cos_lat = rint(BFP_OF_REAL(cosf(RAD_OF_EM7RAD((float)def->lla.lat)), HIGH_RES_TRIG_FRAC));
  int32_t sin_lon = rint(BFP_OF_REAL(sinf(RAD_OF_EM7RAD((float)def->lla.lon)), HIGH_RES_TRIG_FRAC));
  int32_t cos_lon = rint(BFP_OF_REAL(cosf(RAD_OF_EM7RAD((float)def->lla.lon)), HIGH_RES_TRIG_FRAC));
#else
  int32_t sin_lat = rint(BFP_OF_REAL(sin(RAD_OF_EM7RAD((double)def->lla.lat)), HIGH_RES_TRIG_FRAC));
  int32_t cos_lat = rint(BFP_OF_REAL(cos(RAD_OF_EM7RAD((double)def->lla.lat)), HIGH_RES_TRIG_FRAC));
  int32_t sin_lon = rint(BFP_OF_REAL(sin(RAD_OF_EM7RAD((double)def->lla.lon)), HIGH_RES_TRIG_FRAC));
  int32_t cos_lon = rint(BFP_OF_REAL(cos(RAD_OF_EM7RAD((double)def->lla.lon)), HIGH_RES_TRIG_FRAC));
#endif


  def->ltp_of_ecef.m[0] = -sin_lon;
  def->ltp_of_ecef.m[1] =  cos_lon;
  def->ltp_of_ecef.m[2] =  0; /* this element is always zero http://en.wikipedia.org/wiki/Geodetic_system#From_ECEF_to_ENU */
  def->ltp_of_ecef.m[3] = (int32_t)((-(int64_t)sin_lat*(int64_t)cos_lon)>>HIGH_RES_TRIG_FRAC);
  def->ltp_of_ecef.m[4] = (int32_t)((-(int64_t)sin_lat*(int64_t)sin_lon)>>HIGH_RES_TRIG_FRAC);
  def->ltp_of_ecef.m[5] =  cos_lat;
  def->ltp_of_ecef.m[6] = (int32_t)(( (int64_t)cos_lat*(int64_t)cos_lon)>>HIGH_RES_TRIG_FRAC);
  def->ltp_of_ecef.m[7] = (int32_t)(( (int64_t)cos_lat*(int64_t)sin_lon)>>HIGH_RES_TRIG_FRAC);
  def->ltp_of_ecef.m[8] =  sin_lat;

}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------



/**************************************************************/
/**************************************************************/
/*         Convert coordinate from NED to others              */
/**************************************************************/
/**************************************************************/

void ecef_of_ned_pos_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct NedCoor_i* ned)
{
  struct EnuCoor_i enu;
  ENU_OF_TO_NED(enu, *ned);
  ecef_of_enu_pos_i(ecef, def, &enu);
}

/** Convert a local ENU position to ECEF.
 * @param[out] ecef ECEF position in cm
 * @param[in]  def  local coordinate system definition
 * @param[in]  enu  ENU position in meter << #INT32_POS_FRAC
 */
void ecef_of_enu_pos_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu)
{
  /* enu_cm = (enu * 100) >> INT32_POS_FRAC
   * to loose less range:
   * enu_cm = (enu * 25) >> (INT32_POS_FRAC-2)
   * which puts max enu input Q23.8 range to 8388km / 25 = 335km
   */
  struct EnuCoor_i enu_cm;
  VECT3_SMUL(enu_cm, *enu, 25);
  INT32_VECT3_RSHIFT(enu_cm, enu_cm, INT32_POS_FRAC-2);
  ecef_of_enu_vect_i(ecef, def, &enu_cm);
  INT32_VECT3_ADD(*ecef, def->ecef);
}

void ecef_of_enu_vect_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu)
{

  const int64_t tmpx = (int64_t)def->ltp_of_ecef.m[0] * enu->x +
                       (int64_t)def->ltp_of_ecef.m[3] * enu->y +
                       (int64_t)def->ltp_of_ecef.m[6] * enu->z;
  ecef->x = (int32_t)(tmpx>>HIGH_RES_TRIG_FRAC);

  const int64_t tmpy = (int64_t)def->ltp_of_ecef.m[1] * enu->x +
                       (int64_t)def->ltp_of_ecef.m[4] * enu->y +
                       (int64_t)def->ltp_of_ecef.m[7] * enu->z;
  ecef->y = (int32_t)(tmpy>>HIGH_RES_TRIG_FRAC);

  /* first element is always zero http://en.wikipedia.org/wiki/Geodetic_system#From_ENU_to_ECEF */
  const int64_t tmpz = (int64_t)def->ltp_of_ecef.m[5] * enu->y +
                       (int64_t)def->ltp_of_ecef.m[8] * enu->z;
  ecef->z = (int32_t)(tmpz>>HIGH_RES_TRIG_FRAC);

}

void ecef_of_ned_point_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct NedCoor_i* ned) {
  struct EnuCoor_i enu;
  ENU_OF_TO_NED(enu, *ned);
  ecef_of_enu_point_i(ecef, def, &enu);
}
void ecef_of_enu_point_i(struct EcefCoor_i* ecef, struct LtpDef_i* def, struct EnuCoor_i* enu) {
  ecef_of_enu_vect_i(ecef, def, enu);
  INT32_VECT3_ADD(*ecef, def->ecef);
}



