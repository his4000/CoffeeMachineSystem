#ifndef H_DATASTRUCTURE
#define H_DATASTRUCTURE

#include"Tick.h"
#include"reserve.h"

/* Sensor data structure */
typedef struct sensor{
	int water_amt;  //amount of water
	int coffee_amt;  //amount of coffee been
	int powder_state;  //exist about coffee powder. exist = 1 / not exist = 0
}SENSOR;

/* Current state data structure */
typedef struct currentstate{
	int cur_state;  //current state(waitin, grinding, and so on)
	int cur_amt;  //current concentration
	int cur_temp;  //current temperature
	RESRV_DATA reserve_data;  //reservation data
}CUR_STATE;

/* Overall data structure */
typedef struct overall{
	SENSOR cur_sensor;  //sensor state
	CUR_STATE cur_state;  //current state
}OVERALL;

/* Command structure */
typedef struct command{
	int order;  //command number
	int reserve_type;  //reservation option
	int amount;  //concentration you want change
	int ingredi_type;  //charge option
	int ingredi_amt;  //charge amount
}COMMAND;

/* Calculated data header */
typedef struct calcdataheader{
	int job_code;  //operation number
	int edit_code;  //category of update data
	int error_code;  //error code
}CAL_HEADER;

/* Calculated data */
typedef struct calcdata{
	CAL_HEADER header;  //header
	int new_data;  //update data
}CAL_DATA;

/* Motor data */
typedef struct motor{
	int motor_job;
	int temp;  //temperature
	int grind;  //need grinding?
}MOTOR_DATA;

#endif
