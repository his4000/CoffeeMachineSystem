#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"Tick.h"
#include"ErrorHandling.h"

int main(int argc, char* argv[]){
	TIME time;
	FILE* fp;
	
	/* Default time setting */
	if(argc != 4){
		time.hour = 0;
		time.min = 0;
		time.sec = 0;
	}

	/* Time setting */
	else{
		time.hour = atoi(argv[1]);
		time.min = atoi(argv[2]);
		time.sec = atoi(argv[3]);
	}

	/* Time error check */
	if((time.hour) > 24 || (time.hour) < 0)
		DieWithError("Input hour error",0,0,4);
	if((time.min) >= 60 || (time.min) < 0)
		DieWithError("Input minute error",0,0,4);
	if((time.sec) >= 60 || (time.sec) < 0)
		DieWithError("Input second error",0,0,4);
	while(1){
		system("clear");
		/* tick file write */
		fp = fopen(TICK_FILE,"w");
		fwrite(&time,sizeof(TIME),1,fp);
		fclose(fp);

		sleep(1);
		(time.sec)++;
		if((time.sec) == 60){
			(time.sec) = 0;
			(time.min)++;
		}
		if((time.min) == 60){
			(time.min) = 0;
			(time.hour)++;
		}
		if((time.hour) == 24)
			(time.hour) = 0;
	}
	return 0;
}
