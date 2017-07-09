#ifndef H_TICK
#define H_TICK

#define TICK_FILE "tick.dat"

typedef struct my_time{
	int hour;
	int min;
	int sec;
}TIME;

void setting_time(TIME*,int,int,int);

void setting_time(TIME* time, int hour, int min, int sec){
	time->hour = hour;
	time->min = min;
	time->sec = sec;
}
#endif
