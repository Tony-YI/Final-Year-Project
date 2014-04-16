#ifndef _MAIN_H
#define _MAIN_H

#define _USE_MBED 0

//-------------------------------------------//

#if _USE_MBED

#else

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <fcntl.h>
#include "Serial.h"


typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef unsigned long long uint64_t;

#define MAX_UINT64_T 0xffffffffffffffff

#define my_write(fd,buff,size) write(fd,buff,size)
#define my_read(fd,buff,size) read(fd,buff,size) 

//for POSIX timer, should link with -lrt
#include <signal.h>
//#include <time.h>
#include <sys/time.h>

#include <pthread.h>
//this mutex indicates whether the quadcopters status if currently being modified by data handler
extern pthread_mutex_t quad_status_readable;
//this mutex indicates whether GCS is busy executing an command.
extern pthread_mutex_t GCS_busy;

extern pthread_mutex_t XBEE_WRITE;

extern pthread_mutex_t XBEE_READ;
#endif
//-------------------------------------------//

#define KB 1024
#define MB KB * KB

#endif
