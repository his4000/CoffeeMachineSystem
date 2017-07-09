#ifndef H_RESERVE
#define H_RESERVE

#include"Tick.h"

#define RESRV_FILE "reserve.dat"

typedef struct reserve{
	TIME res_coffee;  //making time
	TIME res_cleaning;  //cleaning time
	int is_coffee;  //is coffee making reserved?
	int is_cleaning;  //is cleaning reserved?
}RESRV_DATA;

#endif
