#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[]){
	pid_t pid;
	char hour[2] = "00";
	char min[2] = "00";
	char sec[2] = "00";
	char tick_command[16] = "./Tick ";

	strcpy(hour, argv[1]);
	strcpy(min, argv[2]);
	strcpy(sec, argv[3]);
	
	strcat(tick_command,hour);
	strcat(tick_command," ");
	strcat(tick_command,min);
	strcat(tick_command," ");
	strcat(tick_command,sec);

	pid = fork();

	if(pid == 0)
		system("./cmd_sel");
	else
		system(tick_command);


	return 0;
}
