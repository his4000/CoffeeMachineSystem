#ifndef H_CONSOLE
#define H_CONSOLE

#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>
#include"Tick.h"

#define ENTER 10
#define SPACE 32

void gotoxy(int,int);  //goto (x, y) point in console
int my_getch(void);  //getch function
int my_kbhit(void);  //kbhit function
void my_wait(const char);  //waiting key input
void my_sleep(int);  //sleep for seconds
void my_sleep_sec();  //sleep for 1 second

void gotoxy(int x, int y){
	printf("\033[%dd\033[%dG",y,x);
}

int my_getch(void){
	struct termios oldt,
	newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int my_kbhit(void){
	struct termios oldt, newt;
	int ch;
	int oldf;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO,F_GETFL,0);
	fcntl(STDIN_FILENO,F_SETFL, oldf | O_NONBLOCK);
	
	ch = my_getch();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch!=EOF){
		return ch;
	}

	return 0;
}

void my_wait(const char opt){
	while(my_getch() != opt);
}

void my_sleep(int sec){
	int i;
	
	for(i=0;i<sec;i++)
		my_sleep_sec();
}

void my_sleep_sec(){
	FILE* fp;
	TIME time;
	int sec;

	/* Read now time */
	fp = fopen(TICK_FILE,"r");
	fread(&time,sizeof(TIME),1,fp);
	fclose(fp);

	sec = time.sec;  //input now sec

	while(1){
		fp = fopen(TICK_FILE,"r");
		fread(&time,sizeof(TIME),1,fp);
		fclose(fp);

		if(sec == 59){
			if(time.sec == 0)
				break;
		}
		else{
			if(time.sec == sec+1)
				break;
		}
	}
}

#endif
