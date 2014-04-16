#include "main.h"
#include "Ground_Station.h"
#include "signal.h"


Ground_Station * GCS;
//mutex indicating whether the status of quadcopters are readable
pthread_mutex_t quad_status_readable = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t GCS_busy = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t XBEE_WRITE = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t XBEE_READ = PTHREAD_MUTEX_INITIALIZER;

void kill_all_quads(int32_t signum)
{
	printf("killing all quads\n");
	GCS->ap_kill_quadcopter();
	GCS->wait_all_quads(SWARM_KILLED);
	exit(1);
}

int main(int argc, char **argv)
{
	char default_portname[32] = "/dev/ttyUSB0"; 
	if(argc == 2)
		GCS = new Ground_Station(argv[1],argc,argv);	
	else	
		GCS = new Ground_Station(default_portname,argc,argv);
	
	pthread_t tid = 0;
	pthread_attr_t thread_attr;
	
	pthread_mutex_init(&quad_status_readable,NULL);
	pthread_mutex_init(&GCS_busy,NULL);
	pthread_mutex_init(&XBEE_READ,NULL);
	pthread_mutex_init(&XBEE_WRITE,NULL);
		
	printf("creating thread for data handler\n");	
	pthread_attr_init(&thread_attr);
	pthread_create(&tid,&thread_attr,GCS->periodic_data_handle,NULL);
	
	GCS->GCS_GUI->GUI_main();
	
	return 0;
}
