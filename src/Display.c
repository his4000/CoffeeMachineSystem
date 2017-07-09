#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"datastructure.h"
#include"Display.h"
#include"GUI.h"
#include"Console.h"

int main(void){
	OVERALL overall_data;
	FILE* fp;

	system("clear");

	printDisplayFrame();

	while(1){
		/* read overall data from overall data file */
		fp = fopen(OVERALL_FILE,"r");
		if(fp == NULL){
			printf("  Please execute Main.exe first\n\n");
			printf("  And press ENTER\n");
			my_wait(ENTER);
			gotoxy(0,DISP_TOP+DISP_HEIGHT);
			printf("                                        \n\n");
			printf("                                        \n");
			fp = fopen(OVERALL_FILE,"r");
		}
		fread(&overall_data,sizeof(OVERALL),1,fp);
		fclose(fp);
		
		printDisplayContents(&overall_data);
	}
	return 0;
}
