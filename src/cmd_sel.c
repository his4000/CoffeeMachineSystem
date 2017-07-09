#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"Tick.h"
#include"reserve.h"
#include"cmd_sel.h"

int time_checker();  //reservation time checker

int main(void){
	FILE* fp;
	int result=-1;  //time check result

	while(1){
		fp = fopen(TIME_CHECK_FILE,"w");
			
		result = time_checker();  //time check
		fwrite(&result,sizeof(int),1,fp);  //write time check result in file
		fclose(fp);
	}

	return 0;
}

int time_checker(){
	FILE* tick;
	FILE* reserve;
	TIME now_time;
	RESRV_DATA resrv_data;
	
	tick = fopen(TICK_FILE,"r");
	reserve = fopen(RESRV_FILE,"r");

	while(tick == NULL)  //if tick file open fail
		tick = fopen(TICK_FILE,"r");  //busy waiting
	while(reserve == NULL)  //if reserve file open fail
		reserve = fopen(RESRV_FILE,"r");  //busy waiting
	
	fread(&now_time,sizeof(TIME),1,tick);
	fread(&resrv_data,sizeof(RESRV_DATA),1,reserve);

	fclose(tick);
	fclose(reserve);
	
	/* check coffee making reservation */
	if(resrv_data.is_coffee == 1){
		if(now_time.hour == resrv_data.res_coffee.hour){
			if(now_time.min == resrv_data.res_coffee.min)
				return 1;  //it's making time!!!
		}
	}
	
	/* check cleaning reservation */
	if(resrv_data.is_cleaning == 1){
		if(now_time.hour == resrv_data.res_cleaning.hour){
			if(now_time.min == resrv_data.res_cleaning.min)
				return 2;  //it's cleaning time!!!
		}
	}
	return 0;  //it's not reservation time!!!
}
