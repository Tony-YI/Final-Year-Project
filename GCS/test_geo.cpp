#include "geo.h"
#include "stdio.h"

int main()
{
	struct LtpDef_i ref;
	struct EcefCoor_i ecef_ref;
	ecef_ref.x = -241887298;
	ecef_ref.y = 538037726;
	ecef_ref.z = 241732905;

	ltp_def_from_ecef_i(&ref,&ecef_ref);
	struct EcefCoor_i pos1;
	pos1.x = -241887285;
	pos1.y = 538037716;
	pos1.z = 241732895;

	struct NedCoor_i result1;
	ned_of_ecef_pos_i(&result1,&ref,&ecef_ref);
    printf("x %f, ",POS_FLOAT_OF_BFP(result1.x));
    printf("y %f, ",POS_FLOAT_OF_BFP(result1.y));
    printf("z %f\n",POS_FLOAT_OF_BFP(result1.z));
    
	struct NedCoor_i result;
	ned_of_ecef_pos_i(&result,&ref,&pos1);
	printf("x %f, ",POS_FLOAT_OF_BFP(result.x));
	printf("y %f, ",POS_FLOAT_OF_BFP(result.y));
	printf("z %f\n",POS_FLOAT_OF_BFP(result.z));
	
	struct NedCoor_i ned_tar1, ned_tar2;
	
	ned_tar1.x = 100 + POS_FLOAT_OF_BFP(result.x) * 100;
	ned_tar1.y = POS_FLOAT_OF_BFP(result.y) * 100;
	ned_tar1.z = 100 + POS_FLOAT_OF_BFP(result.z) * 100;
	
	ned_tar2.x = -300;
	ned_tar2.y = 0;
	ned_tar2.z = 100;
	
	struct EcefCoor_i ecef_tar1, ecef_tar2;
	ecef_of_ned_point_i(&ecef_tar1,&ref,&ned_tar1);
	ecef_of_ned_point_i(&ecef_tar2,&ref,&ned_tar2);
	
	printf("ecef_tar1 x %d y %d z %d\n",ecef_tar1.x,ecef_tar1.y,ecef_tar1.z);
	printf("ecef_tar2 x %d y %d z %d\n",ecef_tar2.x,ecef_tar2.y,ecef_tar2.z);
	
	ned_of_ecef_pos_i(&result,&ref,&ecef_tar1);
	printf("x %f, ",POS_FLOAT_OF_BFP(result.x));
	printf("y %f, ",POS_FLOAT_OF_BFP(result.y));
	printf("z %f\n",POS_FLOAT_OF_BFP(result.z));
	ned_of_ecef_pos_i(&result,&ref,&ecef_tar2);
	printf("x %f, ",POS_FLOAT_OF_BFP(result.x));
	printf("y %f, ",POS_FLOAT_OF_BFP(result.y));
	printf("z %f\n",POS_FLOAT_OF_BFP(result.z));
	
	ned_of_ecef_point_i(&result,&ref,&ecef_tar1);
	printf("x %f, ",POS_FLOAT_OF_BFP(result.x));
	printf("y %f, ",POS_FLOAT_OF_BFP(result.y));
	printf("z %f\n",POS_FLOAT_OF_BFP(result.z));
	ned_of_ecef_point_i(&result,&ref,&ecef_tar2);
	printf("x %f, ",POS_FLOAT_OF_BFP(result.x));
	printf("y %f, ",POS_FLOAT_OF_BFP(result.y));
	printf("z %f\n",POS_FLOAT_OF_BFP(result.z));
	return 0;
}
