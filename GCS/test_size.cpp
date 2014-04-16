#include "stdio.h"
#include "main.h"

int main()
{
	/*
	typedef unsigned int uint32_t;
	typedef int int32_t;
	typedef unsigned short uint16_t;
	typedef short int16_t;
	typedef unsigned char uint8_t;
	typedef unsigned long long uint64_t;
	*/
	printf("sizeof(uint32_t) is %lu\n",sizeof(uint32_t));
	printf("sizeof(int32_t) is %lu\n",sizeof(int32_t));
	printf("sizeof(uint16_t) is %lu\n",sizeof(uint16_t));
	printf("sizeof(int16_t) is %lu\n",sizeof(int16_t));
	printf("sizeof(uint8_t) is %lu\n",sizeof(uint8_t));
	printf("sizeof(int8_t) is %lu\n",sizeof(int8_t));
	printf("sizeof(uint64_t) is %lu\n",sizeof(uint64_t));
	printf("sizeof(int64_t) is %lu\n",sizeof(int64_t));

	printf("sizeof(float) is %lu\n",sizeof(float));
	printf("sizeof(double) is %lu\n",sizeof(double));
	return 0;
}
