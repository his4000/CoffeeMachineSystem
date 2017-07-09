#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<sys/stat.h>
#include"GUI.h"
#include"Console.h"
#include"ErrorHandling.h"
#include"Tick.h"
#include"reserve.h"
#include"cmd_sel.h"
#include"datastructure.h"
#include"Display.h"

#define SNR_FILE "sensor.dat"
#define CUR_FILE "current_state.dat"

#define CMD_SIZE 32  //command string size

#define SNR_WATER 1000  //init water amount
#define SNR_BEAN 100  //init coffee bean amount

#define MAX_WATER 1000  //max amount of water
#define MAX_BEAN 100  //max amount of coffee bean

void sensor_recv(SENSOR*);  //recevie sensor data from file
void command_recv(char*);  //receiver command from user

void command_selector(COMMAND*);  //command selector
void input_command(COMMAND*);  //transition command line to command structure
void reservation_command(COMMAND*,int);  //transition reservation command to command structure

void init_sensor(SENSOR*);  //sensor data initializing
void write_sensor(SENSOR*);  //write sensor data in file
void read_sensor(SENSOR*);  //read sensor data from file

void init_current_state(CUR_STATE*);  //current state file initializing
void read_current_state(CUR_STATE*);  //read current state file

void state_integration(OVERALL*, SENSOR*, CUR_STATE*);  //state integration process
void copy_sensor_data(OVERALL*, SENSOR*);  //copy sensor data to overall data
void copy_cur_state_data(OVERALL*, CUR_STATE*);  //copy current state data to overall data
void copy_overall_data(OVERALL*, SENSOR*, CUR_STATE*);  //copy overall data to sensor data and current state data

void reservation_input(COMMAND*);  //reservation data input
void amount_input(COMMAND*);  //concentration change data input
void charge_input(COMMAND*);  //charge ingredient data input

void calculator(COMMAND*);  //calculator

void making_check(int,int,int,int,int);  //making coffee check
void reservation_interface(int,int);  //reservation interface
void cleaning_check(int,int);  //cleaning check
void amount_check(int,int);  //amount check
void hot_ice_check(int);  //hot_ice_check
void powder_cleaning(int,int);  //powder cleaning check
void charge_check(int,int,int);  //ingredient charge

void update(CAL_DATA*);  //update overall data
void making_update(CAL_DATA*,OVERALL*);  //coffee making update
void reserve_update(CAL_DATA*,OVERALL*);  //reservation update
void cleaning_update(CAL_DATA*,OVERALL*);  //cleaning update
void amount_update(CAL_DATA*,OVERALL*);  //concentration update
void temperature_update(CAL_DATA*,OVERALL*);  //temperature update
void powder_update(CAL_DATA*,OVERALL*);  //powder update
void charge_update(CAL_DATA*,OVERALL*);  //charge update

void write_overall_data(OVERALL*);  //update overall data

void motor_interface(MOTOR_DATA*);  //motor interface
void making_order();  //making order
void grinding_order();  //grinding order
void cleaning_order();  //cleaning order
void boiling_order();  //boiling order

int main(void){
	SENSOR sensor_data;  //sensor data
	CUR_STATE cur_state;  //current state
	OVERALL overall_data;  //overall data
	COMMAND command;

	while(1){
		printDefaultFrame();  //print default frame

		state_integration(&overall_data, &sensor_data, &cur_state);

		/* Command selector */
		command_selector(&command);
		/* calculating */
		calculator(&command);
	}

	return 0;
}

void sensor_recv(SENSOR* sensor_data){
	struct stat sensor_file;
	
	/* No sensor file */
	if(stat(SNR_FILE, &sensor_file) < 0)
		init_sensor(sensor_data);
	/* sensor file exist */
	else
		read_sensor(sensor_data);
}

void command_recv(char* command){
	while(1){
		gotoxy(CMD_X, CMD_Y);  //goto command input line
		printf("%s",CMD);  //print "command input >> "
		scanf("%s",command);  //recv command from user
		
		while(getchar()!='\n');
		/* command is not correct */
		if(checkCommandError(command) == -1){
			DieWithError("Command is incorrect",ERR_X,ERR_Y,4);
			system("clear");
			printDefaultFrame();
		}
		/* command is correct */
		else
			break;
	}
}

void command_selector(COMMAND* com_str){
	int selected_command = 0;
	FILE* fp;
	int ch=0;

	while(1){
		/* time check */
		fp = fopen(TIME_CHECK_FILE,"r");
		fread(&selected_command,sizeof(int),1,fp);
		fclose(fp);
	
		/* It is not reservation time */
		if(selected_command == 0){
			if((ch = my_kbhit()) == ENTER){
				gotoxy(CMD_X, CMD_Y);
				input_command(com_str);
				return;
			}
			else if(ch != 0){
				system("clear");
				printDefaultFrame();
			}
		}
		/* It is reservation time */
		else{
			reservation_command(com_str,selected_command);
			return;
		}
	}
}

void input_command(COMMAND* com_str){
	char command[CMD_SIZE];

	command_recv(command);  //command rececive

	/* transition command to order number */
	if(strncmp(command,"1",CMD_SIZE) == 0)
		com_str->order = 0;
	else if(strncmp(command,"2",CMD_SIZE) == 0)
		com_str->order = 1;
	else if(strncmp(command,"3",CMD_SIZE) == 0)
		com_str->order = 2;
	else if(strncmp(command,"4",CMD_SIZE) == 0)
		com_str->order = 3;
	else if(strncmp(command,"tog",CMD_SIZE) == 0)
		com_str->order = 4;
	else if(strncmp(command,"@",CMD_SIZE) == 0)
		com_str->order = 5;
	else if(strncmp(command,"clp",CMD_SIZE) == 0)
		com_str->order = 6;
	else if(strncmp(command,"chrg",CMD_SIZE) == 0)
		com_str->order = 7;	

	/* reservation type command input */
	if(com_str->order == 1)
		reservation_input(com_str);
	/* concentration select input */
	else if(com_str->order == 3)
		amount_input(com_str);
	/* charge info input */
	else if(com_str->order == 7)
		charge_input(com_str);
}

void reservation_command(COMMAND* com_str, int selected_command){
	FILE* fp;
	int end = 1;
	RESRV_DATA reserve_data;
	CUR_STATE state;
	OVERALL over;

	/* Read reservation information */
	fp = fopen(RESRV_FILE,"r");
	fread(&reserve_data,sizeof(RESRV_DATA),1,fp);
	fclose(fp);
	/* Read overall data */
	fp = fopen(OVERALL_FILE,"r");
	fread(&over,sizeof(OVERALL),1,fp);
	fclose(fp);
	/* Read current state data */
	fp = fopen(CUR_FILE,"r");
	fread(&state,sizeof(CUR_STATE),1,fp);
	fclose(fp);	

	if(selected_command == 1){
		com_str->order = 0;  //setting command(making)
		reserve_data.is_coffee = 0;  //change reservation info
		state.reserve_data.is_coffee = 0;
	}
	else if(selected_command == 2){
		com_str->order = 2;  //setting command(cleaning)
		reserve_data.is_cleaning = 0;  //change reservation info
		state.reserve_data.is_cleaning = 0;
	}

	copy_cur_state_data(&over,&state);

	/* Update reservation info file */
	fp = fopen(RESRV_FILE,"w");
	fwrite(&reserve_data,sizeof(RESRV_DATA),1,fp);
	fclose(fp);
	/* Update overall data file */
	fp = fopen(OVERALL_FILE,"w");
	fwrite(&over,sizeof(OVERALL),1,fp);
	fclose(fp);
	/* Update current state data file */
	fp = fopen(CUR_FILE,"w");
	fwrite(&state,sizeof(CUR_STATE),1,fp);
	fclose(fp);		
}

void init_sensor(SENSOR* sensor_data){
	FILE* fp = fopen(SNR_FILE,"w");
	
	sensor_data->water_amt = SNR_WATER;
	sensor_data->coffee_amt = SNR_BEAN;
	sensor_data->powder_state = 0;

	fwrite(sensor_data,sizeof(SENSOR),1,fp);

	fclose(fp);
}

void write_sensor(SENSOR* sensor_data){
	FILE* fp = fopen(SNR_FILE,"w");

	fwrite(sensor_data,sizeof(SENSOR),1,fp);

	fclose(fp);
}
void read_sensor(SENSOR* sensor_data){
	FILE* fp = fopen(SNR_FILE,"r");
	
	fread(sensor_data,sizeof(SENSOR),1,fp);

	fclose(fp);
}

void init_current_state(CUR_STATE* cur_state){
	FILE* fp = fopen(CUR_FILE,"w");
	
	cur_state->cur_state = 0;  //waiting
	cur_state->cur_amt = 0;  //concentration
	cur_state->cur_temp = 0;  //temperature
	cur_state->reserve_data.res_coffee.hour = -1;  //not reserved
	cur_state->reserve_data.res_cleaning.hour = -1;  //not reserved

	fwrite(cur_state,sizeof(CUR_STATE),1,fp);
		
	fclose(fp);
}

void read_current_state(CUR_STATE* cur_state){
	FILE* fp = fopen(CUR_FILE,"r");

	fread(cur_state,sizeof(CUR_STATE),1,fp);

	fclose(fp);
}

void state_integration(OVERALL* overall, SENSOR* sensor_data, CUR_STATE* cur_state){
	struct stat current_file;

	/* No current state file */
	if(stat(CUR_FILE, &current_file) < 0)
		init_current_state(cur_state);
	/* current state file exist */
	else
		read_current_state(cur_state);
	/* receive sensor data */
	sensor_recv(sensor_data);
	
	/* copy sensor data */
	copy_sensor_data(overall, sensor_data);
	/* copy current state data */
	copy_cur_state_data(overall, cur_state);
	
	/* overall data write for file */
	write_overall_data(overall);
}

void copy_sensor_data(OVERALL* overall, SENSOR* sensor_data){
	/* copy sensor data */
	overall->cur_sensor.water_amt = sensor_data->water_amt;
	overall->cur_sensor.coffee_amt = sensor_data->coffee_amt;
	overall->cur_sensor.powder_state = sensor_data->powder_state;
}

void copy_cur_state_data(OVERALL* overall, CUR_STATE* cur_state){
	/* copy current state data */
	overall->cur_state.cur_state = cur_state->cur_state;
	overall->cur_state.cur_amt = cur_state->cur_amt;
	overall->cur_state.cur_temp = cur_state->cur_temp;
	overall->cur_state.reserve_data.res_coffee.hour = cur_state->reserve_data.res_coffee.hour;
	overall->cur_state.reserve_data.res_coffee.min = cur_state->reserve_data.res_coffee.min;
	overall->cur_state.reserve_data.is_coffee = cur_state->reserve_data.is_coffee;
	overall->cur_state.reserve_data.res_cleaning.hour = cur_state->reserve_data.res_cleaning.hour;
	overall->cur_state.reserve_data.res_cleaning.min = cur_state->reserve_data.res_cleaning.min;
	overall->cur_state.reserve_data.is_cleaning = cur_state->reserve_data.is_cleaning;
}
void copy_overall_data(OVERALL* overall, SENSOR* sensor, CUR_STATE* state){
	/* copy overall data */
	sensor->water_amt = overall->cur_sensor.water_amt;
	sensor->coffee_amt = overall->cur_sensor.coffee_amt;
	sensor->powder_state = overall->cur_sensor.powder_state;

	state->cur_state = overall->cur_state.cur_state;
	state->cur_amt = overall->cur_state.cur_amt;
	state->cur_temp = overall->cur_state.cur_temp;
	state->reserve_data.res_coffee.hour = overall->cur_state.reserve_data.res_coffee.hour;
	state->reserve_data.res_coffee.min = overall->cur_state.reserve_data.res_coffee.min;
	state->reserve_data.res_cleaning.hour = overall->cur_state.reserve_data.res_cleaning.hour;
	state->reserve_data.res_cleaning.min = overall->cur_state.reserve_data.res_cleaning.min;
	state->reserve_data.is_coffee = overall->cur_state.reserve_data.is_coffee;
	state->reserve_data.is_cleaning = overall->cur_state.reserve_data.is_cleaning;
}
void reservation_input(COMMAND* com_str){
	int option;  //reservation option

	printReserveFrame();  //print reservation frame
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+1);
	printf("0 : 커피 추출 예약");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+3);
	printf("1 : 청소 예약");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+5);
	printf("2 : EXIT");

	do{
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("                                    ");  //erase command line
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("reservation type input >> ");
		scanf("%d",&option);
		while(getchar()!='\n');
		
		/* input error */
		if(option!=0&&option!=1&&option!=2)
			DieWithError("Input Error",ERR_X+5,ERR_Y,4);
	}while(option!=0&&option!=1&&option!=2);
	com_str->reserve_type = option;
	/* input exit */
	if(option == 2)
		com_str->order = 10;
}

void amount_input(COMMAND* com_str){
	int option;  //amount option

	printAmountFrame();  //print concentration frame
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+1);
	printf("0 : 진하게");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+3);
	printf("1 : 중간");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+5);
	printf("2 : 연하게");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+7);
	printf("3 : EXIT");

	do{
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("                              ");  //erase command line
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("amount select >> ");
		scanf("%d",&option);
		while(getchar()!='\n');

		/* input error */
		if(option!=0&&option!=1&&option!=2&&option!=3)
			DieWithError("Input Error",ERR_X+5,ERR_Y,4);
	}while(option!=0 && option!=1 && option!=2 && option!=3);
	com_str->amount = option;
	/* Input exit */
	if(option == 3)
		com_str->order = 10;
}
void charge_input(COMMAND* com_str){
	int type;  //type of ingredient
	int amount;  //amount of ingredient
	FILE* fp;
	OVERALL over;

	/* Read overall data file */
	fp = fopen(OVERALL_FILE,"r");
	fread(&over,sizeof(OVERALL),1,fp);
	fclose(fp);

	over.cur_state.cur_state = 6;  //charging
	write_overall_data(&over);

	printChargeFrame();  //print charge frame
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+1);
	printf("0 : 원두 보충");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+3);
	printf("1 : 물 보충");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+5);
	printf("2 : 커피 가루 보충");
	gotoxy(SUB_MAN_X+2,SUB_MAN_Y+7);
	printf("3 : EXIT");

	do{
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("                                  ");  //erase command line
		gotoxy(SUB_CMD_X,SUB_CMD_Y);
		printf("Ingredient type input >> ");
		scanf("%d",&type);
		while(getchar()!='\n');
		
		/* input error */
		if(type!=0 && type!=1 && type!=2 && type!=3)
			DieWithError("Input Error",ERR_X+5,ERR_Y,4);
		else if(type==0 || type==1){
			/* print reservation time */
			eraseFrame(SUB_MAN_X,SUB_MAN_Y,SUB_MAN_WIDTH,SUB_MAN_HEIGHT);  //erase sub frame manual
			gotoxy(SUB_ACK_X,SUB_ACK_Y);
			printf("Input amount of ingredient");

			do{
				gotoxy(SUB_CMD_X,SUB_CMD_Y);
				printf("                                  ");  //erase command line
				gotoxy(SUB_CMD_X,SUB_CMD_Y);
				printf("amount input >> ");
				scanf("%d",&amount);
				while(getchar()!='\n');
		
				com_str->ingredi_amt = amount;

				/* Input error */
				if(amount<0)
					DieWithError("Input positive number",ERR_X-1,ERR_Y,4);
			}while(amount < 0);
		}
	}while(type!=0 && type!=1 && type!=2 && type!=3);

	com_str->ingredi_type = type;
	/* input exit */
	if(type == 3)
		com_str->order = 10;
	over.cur_state.cur_state = 0;  //waiting
	write_overall_data(&over);
}

void calculator(COMMAND* command){
	OVERALL overall;
	FILE* fp;

	/* read overall data */
	fp = fopen(OVERALL_FILE,"r");
	fread(&overall,sizeof(OVERALL),1,fp);
	fclose(fp);

	/* operation select */
	switch(command->order){
	case 0 :  //coffee making
		making_check(overall.cur_state.cur_state, overall.cur_sensor.water_amt, 
				overall.cur_state.cur_amt, 
				overall.cur_sensor.coffee_amt, overall.cur_sensor.powder_state);
		break;
	case 1 :  //reservation
		reservation_interface(overall.cur_state.cur_state, command->reserve_type);
		break;
	case 2 :  //cleaning
		cleaning_check(overall.cur_state.cur_state, overall.cur_sensor.water_amt);
		break;
	case 3 :  //change concentration
		amount_check(overall.cur_state.cur_state, command->amount);
		break;
	case 4 :  //change temperature
		hot_ice_check(overall.cur_state.cur_state);
		break;
	case 5 :  //power off
		exit(1);
		break;
	case 6 :  //remove coffee powder
		powder_cleaning(overall.cur_state.cur_state, overall.cur_sensor.powder_state);
		break;
	case 7 :  //charge ingredient
		charge_check(overall.cur_state.cur_state, command->ingredi_type, command->ingredi_amt);
		break;
	default :  //default
		break;
	}
}

void making_check(int cur_state, int water_amt, int cur_amt, int coffee_amt, int powder_state){
	CAL_DATA cal_data;  //calculated data
	
	cal_data.header.job_code = 0;  //coffee making

	if(cur_state == 0){  //It's waiting state
		if(water_amt >= (cur_amt*100+100)){
			if(powder_state == 1){
				cal_data.header.error_code = 0;  //there is no error
				cal_data.header.edit_code = 0;  //coffee powder is exist
			}
			else if(powder_state == 0 && coffee_amt >= 10){
				cal_data.header.error_code = 0;  //there is no error
				cal_data.header.edit_code = 1;  //coffee powder is not exist. need grinding
			}
			else if(powder_state == 2)
				cal_data.header.error_code = 4;  //please powder clean
			else
				cal_data.header.error_code = 2;  //lack of coffee
		}
		else
			cal_data.header.error_code = 3;  //lack of water
	}
	else
		cal_data.header.error_code = 1;  //it's not waiting

	update(&cal_data);  //call update interface
}

void reservation_interface(int cur_state, int reserve_type){
	CAL_DATA cal_data;  //calculated data
	int hour, min;  //reservation time input variables
	RESRV_DATA reserve_data;  //reserve data
	FILE* fp;
	struct stat file_check;
	OVERALL overall;
	
	cal_data.header.job_code = 1;  //reservation
	/* No reserve file */
	if(stat(RESRV_FILE, &file_check) < 0){
		fp = fopen(RESRV_FILE,"w");
		fclose(fp);
	}
			
	/* Read from overall data file */
	fp = fopen(OVERALL_FILE,"r");
	fread(&overall,sizeof(OVERALL),1,fp);
	fclose(fp);

	/* Read from reservation file */
	fp = fopen(RESRV_FILE,"r");
	fread(&reserve_data,sizeof(RESRV_DATA),1,fp);
	fclose(fp);

	if(cur_state == 0){  //it's waiting state
		/* Change current state */
		overall.cur_state.cur_state = 4;  //reservation
		write_overall_data(&overall);
		/* print reservation time */
		eraseFrame(SUB_MAN_X,SUB_MAN_Y,SUB_MAN_WIDTH,SUB_MAN_HEIGHT);  //erase sub frame manual
		gotoxy(SUB_ACK_X,SUB_ACK_Y);
		printf("Input reservation time");
		do{
			/* input hour */
			gotoxy(SUB_CMD_X,SUB_CMD_Y);
			printf("                                   ");  //erase command line
			gotoxy(SUB_CMD_X,SUB_CMD_Y);
			printf("hour >> ");
			scanf("%d",&hour);
			while(getchar()!='\n');

			/* hour input error */
			if(hour>23||hour<0)
				DieWithError("Hour input error",ERR_X+2,ERR_Y,4);
			else{
				/* input minute */
				gotoxy(SUB_CMD_X,SUB_CMD_Y);
				printf("                                   ");  //erase command line
				gotoxy(SUB_CMD_X,SUB_CMD_Y);
				printf("min >> ");
				scanf("%d",&min);
				while(getchar()!='\n');				

				/* minute input error */
				if(min>59||min<0)
					DieWithError("Minute input error",ERR_X+1,ERR_Y,4);
			}
			
		}while((hour>23||hour<0) || (min>59||min<0));

		if(reserve_type == 0){  //coffee making reservation
			reserve_data.res_coffee.hour = hour;
			reserve_data.res_coffee.min = min;
			reserve_data.is_coffee = 1;
			cal_data.header.edit_code = 0;
		}
		else if(reserve_type == 1){  //cleaning reservation
			reserve_data.res_cleaning.hour = hour;
			reserve_data.res_cleaning.min = min;
			reserve_data.is_cleaning = 1;
			cal_data.header.edit_code = 1;
		}
		cal_data.header.error_code = 0;  //there is no error
		/* Change current state */
		overall.cur_state.cur_state = 0;  //waiting
		write_overall_data(&overall);
	}
	else
		cal_data.header.error_code = 1;  //it's not waiting

	/* Write to reservation file */
	fp = fopen(RESRV_FILE,"w");
	fwrite(&reserve_data,sizeof(RESRV_DATA),1,fp);
	fclose(fp);

	update(&cal_data);
}

void cleaning_check(int cur_state, int water_amt){
	CAL_DATA cal_data;  //calculated data

	cal_data.header.job_code = 2;  //cleaning

	if(cur_state == 0){  //it's waiting state
		if(water_amt < 500)
			cal_data.header.error_code = 2;  //lack of water
		else
			cal_data.header.error_code = 0;  //there is no error
	}
	else
		cal_data.header.error_code = 1;  //it's not waiting

	update(&cal_data);  //call update interface
}

void amount_check(int cur_state, int amount){
	CAL_DATA cal_data;  //calculated data

	cal_data.header.job_code = 3;  //change concentration

	if(cur_state == 0){
		cal_data.header.error_code = 0;  //there is no error
		cal_data.new_data = amount;  //update infomation
	}
	else
		cal_data.header.error_code = 1;  //it's not waiting
	
	update(&cal_data);  //call update interface
}

void hot_ice_check(int cur_state){
	CAL_DATA cal_data;  //calculated data

	cal_data.header.job_code = 4;  //change temperature

	if(cur_state == 0)
		cal_data.header.error_code = 0;  //there is no error
	else
		cal_data.header.error_code = 1;  //it's not waiting

	update(&cal_data);  //call update interface
}

void powder_cleaning(int cur_state, int powder_state){
	CAL_DATA cal_data;  //calculated data

	cal_data.header.job_code = 5;  //remove coffee powder

	if(cur_state == 0)
		if(powder_state != 2)
			cal_data.header.error_code = 2;  //can't clean powder
		else
			cal_data.header.error_code = 0;  //there is no error
	else
		cal_data.header.error_code = 1;  //it's not waiting

	update(&cal_data);  //call update interface
}

void charge_check(int cur_state, int ingredi_type, int ingredi_amt){
	CAL_DATA cal_data;  //calculated data

	cal_data.header.job_code = 6;  //ingredient charge

	if(cur_state == 0){
		cal_data.header.error_code = 0;  //there is no error

		cal_data.header.edit_code = ingredi_type;
		cal_data.new_data = ingredi_amt;
	}
	else
		cal_data.header.error_code = 1;  //it's not waiting

	update(&cal_data);
}

void update(CAL_DATA* cal_data){
	FILE* fp;
	OVERALL overall;  //overall data
	SENSOR sensor;  //sensor data
	CUR_STATE cur_state;  //current state data
	MOTOR_DATA motor_data;  //motor data

	motor_data.motor_job = 0;  //initializing

	/* read overall data */
	fp = fopen(OVERALL_FILE,"r");
	fread(&overall,sizeof(OVERALL),1,fp);
	fclose(fp);

	/* update */
	switch(cal_data->header.job_code){
	case 0 :  //coffee making
		making_update(cal_data, &overall);
		if(cal_data->header.error_code == 0)  //making coffee
			motor_data.motor_job = 1;
		if(cal_data->header.edit_code == 1)  //need grinding
			motor_data.grind = 0;
		else  //no need grinding
			motor_data.grind = 1;
		break;
	case 1 :  //reservation
		reserve_update(cal_data, &overall);
		break;
	case 2 :  //cleaning
		cleaning_update(cal_data, &overall);
		if(cal_data->header.error_code == 0)  //cleaning
			motor_data.motor_job = 2;
		break;
	case 3 :  //change concentration
		amount_update(cal_data, &overall);
		break;
	case 4 :  //change temperature
		temperature_update(cal_data, &overall);
		break;
	case 5 :  //remove coffee powder
		powder_update(cal_data, &overall);
		break;
	case 6 :  //ingredient charge
		charge_update(cal_data, &overall);
		break;
	}

	copy_overall_data(&overall,&sensor,&cur_state);  //copy overall data

	/* write overall file */
	write_overall_data(&overall);
	
	/* write sensor file */
	fp = fopen(SNR_FILE,"w");
	fwrite(&sensor,sizeof(SENSOR),1,fp);
	fclose(fp);

	/* write current state file */
	fp = fopen(CUR_FILE,"w");
	fwrite(&cur_state,sizeof(CUR_STATE),1,fp);
	fclose(fp);

	motor_data.temp = overall.cur_state.cur_temp;  //input temperature infomation to motor data

	motor_interface(&motor_data);
}

void making_update(CAL_DATA* cal_data, OVERALL* overall){
	int amount = (overall->cur_state.cur_amt)*100+100;  //amount of concentration
	switch(cal_data->header.error_code){
	case 0 : 
		/* Coffee powder exist */
		if(cal_data->header.edit_code == 0){
			overall->cur_sensor.water_amt -= amount;  //discount water amount
			overall->cur_sensor.powder_state = 2;  //need powder cleaning
		}
		/* Coffee powder not exist */
		else if(cal_data->header.edit_code == 1){
			overall->cur_sensor.coffee_amt -= 10;  //grinding coffee bean
			overall->cur_sensor.water_amt -= amount;  //discount water amount
			overall->cur_sensor.powder_state = 2;  //need powder cleaning
		}
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	case 2 : 
		DieWithError("Lack of coffee",ERR_X+3,ERR_Y,1);
		break;
	case 3 : 
		DieWithError("Lack of water",ERR_X+4,ERR_Y,0);
		break;
	case 4 : 
		DieWithError("Please clean powder",ERR_X+1,ERR_Y,2);
		break;
	}
}

void reserve_update(CAL_DATA* cal_data, OVERALL* overall){
	FILE* fp;
	RESRV_DATA reserve_data;

	switch(cal_data->header.error_code){
	case 0 :
		/* read updated reservation data */
		fp = fopen(RESRV_FILE,"r");
		fread(&reserve_data,sizeof(RESRV_DATA),1,fp);
		fclose(fp);

		if(cal_data->header.edit_code == 0){  //coffee making reservation
			/* copy to overall data */
			overall->cur_state.reserve_data.res_coffee.hour = reserve_data.res_coffee.hour;
			overall->cur_state.reserve_data.res_coffee.min = reserve_data.res_coffee.min;
			overall->cur_state.reserve_data.is_coffee = 1;
		}
		else if(cal_data->header.edit_code == 1){  //cleaning reservation
			/* copy to overall data */
			overall->cur_state.reserve_data.res_cleaning.hour = reserve_data.res_cleaning.hour;
			overall->cur_state.reserve_data.res_cleaning.min = reserve_data.res_cleaning.min;
			overall->cur_state.reserve_data.is_cleaning = 1;
		}
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	}
}

void cleaning_update(CAL_DATA* cal_data, OVERALL* overall){
	switch(cal_data->header.error_code){
	case 0 :
		overall->cur_sensor.water_amt -= 500; 
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	case 2 : 
		DieWithError("Lack of water",ERR_X+4,ERR_Y,0);
		break;
	}
}

void amount_update(CAL_DATA* cal_data, OVERALL* overall){
	switch(cal_data->header.error_code){
	case 0 : 
		overall->cur_state.cur_amt = cal_data->new_data;
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	}
}

void temperature_update(CAL_DATA* cal_data, OVERALL* overall){
	switch(cal_data->header.error_code){
	case 0 : 
		/* Change Hot <-> Ice */
		if(overall->cur_state.cur_temp == 0)
			overall->cur_state.cur_temp = 1;
		else if(overall->cur_state.cur_temp == 1)
			overall->cur_state.cur_temp = 0;
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	}
}

void powder_update(CAL_DATA* cal_data, OVERALL* overall){
	switch(cal_data->header.error_code){
	case 0 : 
		overall->cur_sensor.powder_state = 0;
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	case 2 : 
		DieWithError("Can't clean powder",ERR_X+1,ERR_Y,4);
		break;
	}
}

void charge_update(CAL_DATA* cal_data, OVERALL* overall){
	switch(cal_data->header.error_code){
	case 0 : 
		if(cal_data->header.edit_code == 0){  //coffee bean charge
			if(overall->cur_sensor.coffee_amt + cal_data->new_data > MAX_BEAN)  //exceed max
				DieWithError("Exceed Max Coffee Bean",ERR_X,ERR_Y,4);
			else
				overall->cur_sensor.coffee_amt += cal_data->new_data;
		}
		else if(cal_data->header.edit_code == 1){  //water charge
			if(overall->cur_sensor.water_amt + cal_data->new_data > MAX_WATER)  //exceed max
				DieWithError("Exceed Max Water",ERR_X,ERR_Y,4);
			else
				overall->cur_sensor.water_amt += cal_data->new_data;
		}	
		else if(cal_data->header.edit_code == 2){  //powder charge
			if(overall->cur_sensor.powder_state == 0){  //powder not exist
				overall->cur_sensor.powder_state = 1;  //charge powder
			}
			else
				DieWithError("Powder already exists",ERR_X,ERR_Y,4);
		}
		break;
	case 1 : 
		DieWithError("No waiting",ERR_X,ERR_Y,4);
		break;
	}
}

void write_overall_data(OVERALL* overall){
	FILE* fp;
	
	/* write overall data file */
	fp = fopen(OVERALL_FILE,"w");
	fwrite(overall,sizeof(OVERALL),1,fp);
	fclose(fp);
}

void motor_interface(MOTOR_DATA* motor){
	FILE* fp;
	OVERALL overall;

	/* Read from overall data file */
	fp = fopen(OVERALL_FILE,"r");
	fread(&overall,sizeof(OVERALL),1,fp);
	fclose(fp);

	/* making coffee */
	if(motor->motor_job == 1){
		if(motor->temp == 0){  //Hot
			overall.cur_state.cur_state = 2;  //boiling
			write_overall_data(&overall);
			boiling_order();
		}
		if(motor->grind == 0){  //need grinding
			overall.cur_state.cur_state = 1;  //grinding
			write_overall_data(&overall);
			grinding_order();
			overall.cur_sensor.powder_state = 1;  //powder exists
			write_overall_data(&overall);
		}
		overall.cur_state.cur_state = 3;  //making
		write_overall_data(&overall);
		making_order();
		overall.cur_state.cur_state = 0;  //waiting
		write_overall_data(&overall);
	}
	/* cleaning */
	else if(motor->motor_job == 2){
		overall.cur_state.cur_state = 5;  //cleaning
		write_overall_data(&overall);
		cleaning_order();
		overall.cur_state.cur_state = 0;  //waiting
		write_overall_data(&overall);
	}
}

void making_order(){
	printMakingCoffee();
}

void grinding_order(){
	printGrinding();
}

void cleaning_order(){
	printCleaning();
}

void boiling_order(){
	printBoiling();
}
