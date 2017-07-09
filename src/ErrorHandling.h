#ifndef H_ERRORHANDLING
#define H_ERRORHANDLING

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include"GUI.h"
#include"Console.h"

#define CMD_NUM 8
#define CMD_LENGTH 32

void DieWithError(char*,int,int,int);  //print error message
int checkCommandError(char*);  //check input command is correct
void changeUpperToLower(char*,int);  //change upper alphabat to lower alphabat

void DieWithError(char* errorMessage,int x, int y, int alarm){
	printFrame(ERR_LEFT,ERR_TOP,ERR_WIDTH,ERR_HEIGHT);  //print error frame

	gotoxy(x, y);
	printf("%s",errorMessage);

	gotoxy(ERR_X+3,ERR_Y+2);
	printf("< Press ENTER >");
	if(alarm == 0 || alarm == 1 || alarm == 2)
		printBeep(alarm);
	my_wait(ENTER);

	eraseFrame(ERR_LEFT,ERR_TOP,ERR_WIDTH,ERR_HEIGHT);  //erase error frame
	eraseFrame(ARM_LEFT,ARM_TOP,ARM_WIDTH,ARM_HEIGHT);  //erase alarm frame
}

int checkCommandError(char* command){
	char command_info[CMD_NUM][CMD_LENGTH] = {"1","2","3","4","tog","@","clp","chrg"};  //setting command info
	int i;

	changeUpperToLower(command, CMD_LENGTH);  //change to lower alphabat

	for(i=0;i<CMD_NUM;i++){
		if(strncmp(command, command_info[i], CMD_LENGTH) == 0)
			return 1;  //command is correct
	}
	return -1;
}

void changeUpperToLower(char* command, int size){
	int i=0;
	
	while(command[i] != '\0' && i < size){
		if(isalpha(command[i])){
			if(isupper(command[i]))
				command[i] = tolower(command[i]);
		}
		i++;
	}
}
#endif
