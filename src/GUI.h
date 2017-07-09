#ifndef H_GUI
#define H_GUI

#include<stdio.h>
#include<stdlib.h>
#include"Console.h"
#include"datastructure.h"

#define STR_SIZE 32  //string size

/* Main Frame */
#define WIDTH 80  //width
#define HEIGHT 30  //height

#define LEFT 2  //Left start point
#define TOP 1  //Top start point 

/* Command line */
#define CMD_X LEFT+1  //command input x point
#define CMD_Y TOP+HEIGHT-2  //command input y point

/* Subject line */
#define SBJ_X LEFT+(WIDTH)/2-6  //subject line x point
#define SBJ_Y TOP+1  //subject line y point

/* Manual frame */
#define MAN_WIDTH (WIDTH)/2 - 1
#define MAN_HEIGHT (2*(HEIGHT))/3

#define MAN_X (LEFT)+((WIDTH)/2)
#define MAN_Y (SBJ_Y)+2

/* Error frame */
#define ERR_WIDTH (WIDTH)/3
#define ERR_HEIGHT (HEIGHT)/3
#define ERR_LEFT (LEFT)+(WIDTH)+1
#define ERR_TOP (TOP)+3*(HEIGHT)/4
#define ERR_X (ERR_LEFT)+(ERR_WIDTH)/6-1
#define ERR_Y (ERR_TOP)+(ERR_HEIGHT)/2-1

/* Alarm frame */
#define ARM_WIDTH (ERR_WIDTH)
#define ARM_HEIGHT (ERR_HEIGHT)
#define ARM_LEFT (ERR_LEFT)
#define ARM_TOP (ERR_TOP)+(ERR_HEIGHT)+1
#define ARM_TEXT_X (ARM_LEFT)+3
#define ARM_TEXT_Y (ARM_TOP)+3

/* Display */
#define DISP_NUM_BLANK 11  //number of blank
#define DISP_INTERVAL 2  //display interval
#define DISP_WIDTH 60  //width
#define DISP_HEIGHT 1+((DISP_INTERVAL)*(DISP_NUM_BLANK))  //height
#define DISP_PARTITION (DISP_LEFT)+(DISP_WIDTH)/3  //display table column partition

#define DISP_LEFT (LEFT)  //left
#define DISP_TOP (TOP)  //top

#define CMD "Command input >> "  //command line sentence
#define MainFrameName "MAIN CONSOLE"  //main frame name

/* Sub command frame */
#define SUB_LEFT (LEFT)
#define SUB_TOP (TOP)+(HEIGHT)+2
#define SUB_WIDTH (WIDTH)
#define SUB_HEIGHT (2*(HEIGHT))/3
#define SUB_CMD_X (CMD_X)  //sub command frame command line x
#define SUB_CMD_Y (SUB_TOP)+(SUB_HEIGHT)-2  //sub command frame command line y
#define SUB_ACK_X SUB_LEFT+SUB_WIDTH/2-12
#define SUB_ACK_Y SUB_TOP+SUB_HEIGHT/2

/* Sub command manual frame */
#define SUB_MAN_WIDTH (SUB_WIDTH)/2 - 1
#define SUB_MAN_HEIGHT (SUB_HEIGHT)/2+1
#define SUB_MAN_X (SUB_LEFT)+((SUB_WIDTH)/2)
#define SUB_MAN_Y (SUB_TOP)+3

void printFrame(int,int,int,int);  //print basic frame
void eraseFrame(int,int,int,int);  //erase frame

void printDefaultFrame();  //print default frame
void printManualChart();  //print manual chart

void printDisplayFrame();  //print Display frame
void printDisplayContents(OVERALL*);  //print Display contents
void printStatus(int);  //print status in display
void printConcentration(int);  //print concentration in display

void printSubFrame(char*,int,int);  //print sub frame
void printSubFrameManualChart();  //print sub frame manual

void printReserveFrame();  //print Reservation interface frame
void printAmountFrame();  //print Change Concentration frame
void printChargeFrame();  //print Charge frame

void printErrorFrame();  //print Error frame

void printMakingCoffee();  //print making coffee
void printGrinding();  //print grinding
void printCleaning();  //print cleaning
void printBoiling();  //print boiling

void printMug();  //print mug
void printBean();  //print bean to powder
void printBroomRight(int);  //print right path broom
void printBroomLeft(int);  //print left path broom
void printPotRight();  //print boiling pot right
void printPotLeft();  //print boiling pot left

void printBeep(int);  //print Beef
void waterBeep();  //lack of water beef
void coffeeBeep();  //lack of coffee beef
void powderBeep();  //need cleaning powder beef

void printFrame(int left, int top, int width, int height){
	int i,j;

	gotoxy(left, top);
	printf("┌");
	for(i=0;i<width-2;i++)
		printf("─");
	printf("┐");

	for(j=0;j<height-2;j++){
		gotoxy(left, top+j+1);
		printf("│");
		for(i=0;i<width-2;i++)
			printf(" ");
		printf("│");
	}
	gotoxy(left, top+height-1);
	printf("└");
	for(i=0;i<width-2;i++)
		printf("─");
	printf("┘");
}

void eraseFrame(int left, int top, int width, int height){
	int i,j;

	gotoxy(left, top);
	for(i=1;i<=height;i++){
		for(j=0;j<width;j++)
			printf(" ");
		gotoxy(left, top+i);
	}
}

void printDefaultFrame(){
	int i,j;

	system("clear");  //clear console

	/* print outline */
	printFrame(LEFT, TOP, WIDTH, HEIGHT);
	/* print subject line */
	gotoxy(LEFT,SBJ_Y+1);
	printf("├");
	for(i=0;i<WIDTH-2;i++)
		printf("─");
	printf("┤");
	gotoxy(SBJ_X,SBJ_Y);
	printf("%s",MainFrameName);

	/* print command line */
	gotoxy(CMD_X-1,CMD_Y-1);
	printf("├");
	for(i=0;i<WIDTH-2;i++)
		printf("─");
	printf("┤");

	gotoxy(CMD_X, CMD_Y);
	printf("Press ENTER");

	/* print manual box */
	printManualChart();
} 

void printManualChart(){
	printFrame(MAN_X, MAN_Y, MAN_WIDTH, MAN_HEIGHT);

	gotoxy(MAN_X+2, MAN_Y+1);	
	printf("COMMAND LIST");
	gotoxy(MAN_X+2, MAN_Y+3);
	printf("1 : 커피 추출");
	gotoxy(MAN_X+2, MAN_Y+5);
	printf("2 : 예약");
	gotoxy(MAN_X+2, MAN_Y+7);
	printf("3 : 청소");
	gotoxy(MAN_X+2, MAN_Y+9);
	printf("4 : 농도 변경");
	gotoxy(MAN_X+2, MAN_Y+11);
	printf("tog : 온도 변경");
	gotoxy(MAN_X+2, MAN_Y+13);
	printf("@ : 전원 OFF");
	gotoxy(MAN_X+2, MAN_Y+15);
	printf("clp : 커피 가루 청소");
	gotoxy(MAN_X+2, MAN_Y+17);
	printf("chrg : 재료 보충");
	
}

void printDisplayFrame(){
	int i,j;

	/* print basic frame */
	printFrame(DISP_LEFT, DISP_TOP, DISP_WIDTH, DISP_HEIGHT);	

	for(i=1;i<DISP_NUM_BLANK;i++){
		gotoxy(DISP_LEFT,DISP_TOP+(i*DISP_INTERVAL));
		printf("├");
		for(j=0;j<DISP_WIDTH-2;j++){
			if(j == DISP_PARTITION && i == 1)
				printf("┬");
			else if(j == DISP_PARTITION && i != 1)
				printf("┼");
			else
				printf("─");
		}
		printf("┤");
	}
	gotoxy(DISP_PARTITION+3,DISP_TOP+DISP_HEIGHT-1);
	printf("┴");

	/* Printing schema */
	i = 1;
	gotoxy(DISP_LEFT+DISP_WIDTH/2-6, DISP_TOP+i);
	printf("D I S P L A Y");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("시간");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("상태");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("농도");
	
	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("온도");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("물 잔량");
	
	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("커피(원두) 잔량");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("커피 가루 상태");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("커피 가루 청소 필요");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("커피 추출 예약 시간");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_LEFT+2, DISP_TOP+i);
	printf("청소 예약 시간");

	gotoxy(0,DISP_TOP+DISP_HEIGHT);
}

void printDisplayContents(OVERALL* overall){
	FILE* fp;
	int i=1;
	TIME cur_time;

	/* fill in the blank of display */
	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	fp = fopen(TICK_FILE,"r");
	fread(&cur_time,sizeof(TIME),1,fp);
	fclose(fp);
	printf("%02d : %02d : %02d                  ",cur_time.hour,cur_time.min,cur_time.sec);

	i = i+DISP_INTERVAL;	
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	printStatus(overall->cur_state.cur_state);  //print status

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	printConcentration(overall->cur_state.cur_amt);  //print concentration

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	switch(overall->cur_state.cur_temp){
	case 0 : 
		printf("HOT");
		break;
	case 1 : 
		printf("ICE");
		break;
	}  //print temperature

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	printf("%d ml    ",overall->cur_sensor.water_amt);  //print water remain

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	printf("%d g    ",overall->cur_sensor.coffee_amt);  //print coffee remain

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	if(overall->cur_sensor.powder_state == 1)
		printf("Exist    ");  //print powder exist
	else
		printf("Not Exist");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	if(overall->cur_sensor.powder_state == 2)
		printf("NEED   ");
	else
		printf("NO NEED");

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	/* print coffee making reservation */
	if(overall->cur_state.reserve_data.is_coffee == 0)
		printf("-- : --");
	else
		printf("%02d : %02d",
		overall->cur_state.reserve_data.res_coffee.hour, overall->cur_state.reserve_data.res_coffee.min);

	i = i+DISP_INTERVAL;
	gotoxy(DISP_PARTITION+6,DISP_TOP+i);
	/* print cleaning reservation */
	if(overall->cur_state.reserve_data.is_cleaning == 0)
		printf("-- : --");
	else
		printf("%02d : %02d",
		overall->cur_state.reserve_data.res_cleaning.hour, overall->cur_state.reserve_data.res_cleaning.min);
}

void printStatus(int cur_state){
	switch(cur_state){
	case 0 : 
		printf("대기중");
		break;
	case 1 : 
		printf("분쇄중");
		break;
	case 2 : 
		printf("가열중");
		break;
	case 3 : 
		printf("추출중");
		break;
	case 4 : 
		printf("예약중");
		break;
	case 5 : 
		printf("청소중");
		break;
	case 6 : 
		printf("보충중");
		break;
	}
}

void printConcentration(int cur_amt){
	switch(cur_amt){
	case 0 : 
		printf("진하게");
		break;
	case 1 : 
		printf("중간  ");
		break;
	case 2 : 
		printf("연하게");
		break;
	}
}

void printSubFrame(char* subject, int x, int y){
	int i;

	printFrame(SUB_LEFT,SUB_TOP,SUB_WIDTH,SUB_HEIGHT);

	/* print subject line */
	gotoxy(SUB_LEFT,SUB_TOP+2);
	printf("├");
	for(i=0;i<SUB_WIDTH-2;i++)
		printf("─");
	printf("┤");
	gotoxy(x,y);
	printf("%s",subject);

	/* print command line */
	gotoxy(SUB_LEFT,SUB_CMD_Y-1);
	printf("├");
	for(i=0;i<SUB_WIDTH-2;i++)
		printf("─");
	printf("┤");

	printSubFrameManualChart();
}

void printSubFrameManualChart(){
	printFrame(SUB_MAN_X,SUB_MAN_Y,SUB_MAN_WIDTH,SUB_MAN_HEIGHT);
}

void printReserveFrame(){
	char subject[STR_SIZE] = "RESERVATION INTERFACE";

	printSubFrame(subject,SUB_LEFT+SUB_WIDTH/2-11,SUB_TOP+1);
}

void printAmountFrame(){
	char subject[STR_SIZE] = "CHANGE CONCENTRATION";

	printSubFrame(subject,SUB_LEFT+SUB_WIDTH/2-11,SUB_TOP+1);
}

void printChargeFrame(){
	char subject[STR_SIZE] = "CHARGE INGREDIENTS";

	printSubFrame(subject,SUB_LEFT+SUB_WIDTH/2-10,SUB_TOP+1);
}

void printMakingCoffee(){
	int i;

	eraseFrame(LEFT+1,TOP+3,WIDTH-2,HEIGHT-6);

	for(i=0;i<10;i++){
		printMug(i);
		sleep(1);
	}
}

void printGrinding(){
	int i;	

	eraseFrame(LEFT+1,TOP+3,WIDTH-2,HEIGHT-6);

	for(i=0;i<5;i++){
		printBean(i);
		sleep(1);
	}
}

void printCleaning(){
	int i;

	eraseFrame(LEFT+1,TOP+3,WIDTH-2,HEIGHT-6);
	
	for(i=0;i<5;i++){
		printBroomRight(5*i);
		sleep(1);
	}

	eraseFrame(LEFT+1,TOP+3,WIDTH-2,HEIGHT-6);

	for(i=0;i<5;i++){
		printBroomLeft(5*i);
		sleep(1);
	}
}

void printBoiling(){
	int i;

	eraseFrame(LEFT+1,TOP+3,WIDTH-2,HEIGHT-6);

	for(i=0;i<10;i++){
		if(i%2 == 0){
			printPotRight();
			sleep(1);
		}
		else{
			printPotLeft();
			sleep(1);
		}
	}	
}

void printMug(int low){
	int x = LEFT+15;  //start point x
	int y = TOP+10;  //start point y
	int i;

	gotoxy(x,y);
	printf("■■■■                              ■■■■\n");
	gotoxy(x,y+1);
	printf(" ■■■                              ■■■ ■■■■■\n");
	gotoxy(x,y+2);
	printf(" ■■■                              ■■■■■   ■■\n");
	gotoxy(x,y+3);
	printf(" ■■■                              ■■■     ■■\n");
	gotoxy(x,y+4);
	printf(" ■■■                              ■■■     ■\n");
	gotoxy(x,y+5);
	printf(" ■■■                              ■■■    ■\n");
	gotoxy(x,y+6);
	printf(" ■■■                              ■■■   ■\n");
	gotoxy(x,y+7);
	printf(" ■■■                              ■■■■■■\n");
	gotoxy(x,y+8);
	printf(" ■■■                              ■■■\n");
	gotoxy(x,y+9);
	printf("  ■■■■■■                      ■■■■■■\n");
	gotoxy(x,y+10);
	printf("    ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+11);
	printf("          ■■■■■■■■■■■■■■■■■■\n");

	if(low > 0){
		gotoxy(x,y+9);
		printf("  ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	}
	for(i=1;i<low;i++){
		gotoxy(x+4,y+9-i);
		printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	}	
}

void printBean(int low){
	int x = LEFT+30;
	int y = TOP+10;
	int i;
	
	/* print bean */
	gotoxy(x,y);
	printf("       ■■■ ■■■\n");
	gotoxy(x,y+1);
	printf("    ■■■■■ ■■■■■\n");
	gotoxy(x,y+2);
	printf("   ■■■■■ ■■■■■\n");
	gotoxy(x,y+3);
	printf("   ■■■■ ■■■■■\n");
	gotoxy(x,y+4);
	printf("    ■■ ■■■■\n");
	
	for(i=0;i<low;i++){
		gotoxy(x,y+4-i);
		printf("                             \n");
	}

	/* print powder */
	if(low > 0){
		gotoxy(x,y+11);
		printf("     ● ● ● ●\n");
		if(low > 1){
			gotoxy(x,y+10);
			printf("      ● ● ●\n");
			if(low > 2){
				gotoxy(x,y+9);
				printf("       ● ●\n");
				if(low > 3){
					gotoxy(x,y+8);
					printf("        ●\n");
				}
			}
		}
	}
}

void printBroomRight(int low){
	int x = LEFT+10+low;
	int y = TOP+10;
	int i;	

	gotoxy(x,y);
	printf("     ■■■\n");
	gotoxy(x,y+1);
	printf("      ■■■\n");
	gotoxy(x,y+2);
	printf("       ■■■\n");
	gotoxy(x,y+3);
	printf("        ■■■\n");
	gotoxy(x,y+4);
	printf("         ■■■\n");
	gotoxy(x,y+5);
	printf("          ■■■\n");
	gotoxy(x,y+6);
	printf("           ■■■\n");
	gotoxy(x,y+7);
	printf("     ■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+8);
	printf("     ■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+9);
	printf("     ■ ■ ■ ■ ■ ■ ■ ■\n");
	gotoxy(x,y+10);
	printf("     ■ ■ ■ ■ ■ ■ ■ ■\n");
}
void printBroomLeft(int low){
	int x = LEFT+35-low;
	int y = TOP+10;

	gotoxy(x,y);
	printf("            ■■■     \n");
	gotoxy(x,y+1);
	printf("           ■■■     \n");
	gotoxy(x,y+2);
	printf("          ■■■     \n");
	gotoxy(x,y+3);
	printf("         ■■■     \n");
	gotoxy(x,y+4);
	printf("        ■■■     \n");
	gotoxy(x,y+5);
	printf("       ■■■     \n");
	gotoxy(x,y+6);
	printf("      ■■■     \n");
	gotoxy(x,y+7);
	printf("■■■■■■■■■■■■■■■     \n");
	gotoxy(x,y+8);
	printf("■■■■■■■■■■■■■■■     \n");
	gotoxy(x,y+9);
	printf("■ ■ ■ ■ ■ ■ ■ ■     \n");
	gotoxy(x,y+10);
	printf("■ ■ ■ ■ ■ ■ ■ ■     \n");
}

void printPotRight(){
	int x = LEFT+30;
	int y = TOP+10;
	
	gotoxy(x,y);
	printf("   ■   ■   ■   ■ \n");
	gotoxy(x,y+1);
	printf("    ■   ■   ■   ■ \n");
	gotoxy(x,y+2);
	printf("   ■   ■   ■   ■ \n");
	gotoxy(x,y+3);
	printf("    ■   ■   ■   ■ \n");
	gotoxy(x,y+4);
	printf("   ■   ■   ■   ■ \n");
	gotoxy(x,y+5);
	printf("\n");
	gotoxy(x,y+6);
	printf("       ■■■■■■■\n");
	gotoxy(x,y+7);
	printf("■■■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+8);
	printf(" ■■■■■■■■■■■■■■■■ ■■■■\n");
	gotoxy(x,y+9);
	printf("   ■■■■■■■■■■■■■■■■  ■■\n");
	gotoxy(x,y+10);
	printf("  ■■■■■■■■■■■■■■■    ■■\n");
	gotoxy(x,y+11);
	printf(" ■■■■■■■■■■■■■■■■   ■■\n");
	gotoxy(x,y+12);
	printf(" ■■■■■■■■■■■■■■■■  ■■\n");
	gotoxy(x,y+13);
	printf(" ■■■■■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+14);
	printf(" ■■■■■■■■■■■■■■■■\n");
}

void printPotLeft(){
	int x = LEFT+30;
	int y = TOP+10;

	gotoxy(x,y);
	printf("    ■   ■   ■   ■ \n");
	gotoxy(x,y+1);
	printf("   ■   ■   ■   ■ \n");
	gotoxy(x,y+2);
	printf("    ■   ■   ■   ■ \n");
	gotoxy(x,y+3);
	printf("   ■   ■   ■   ■ \n");
	gotoxy(x,y+4);
	printf("    ■   ■   ■   ■ \n");
	gotoxy(x,y+5);
	printf("\n");
	gotoxy(x,y+6);
	printf("       ■■■■■■■\n");
	gotoxy(x,y+7);
	printf("■■■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+8);
	printf(" ■■■■■■■■■■■■■■■■ ■■■■\n");
	gotoxy(x,y+9);
	printf("   ■■■■■■■■■■■■■■■■  ■■\n");
	gotoxy(x,y+10);
	printf("  ■■■■■■■■■■■■■■■    ■■\n");
	gotoxy(x,y+11);
	printf(" ■■■■■■■■■■■■■■■■   ■■\n");
	gotoxy(x,y+12);
	printf(" ■■■■■■■■■■■■■■■■  ■■\n");
	gotoxy(x,y+13);
	printf(" ■■■■■■■■■■■■■■■■■■■\n");
	gotoxy(x,y+14);
	printf(" ■■■■■■■■■■■■■■■■\n");
}

void printBeep(int type){
	printFrame(ARM_LEFT,ARM_TOP,ARM_WIDTH,ARM_HEIGHT);

	switch(type){
	case 0 :  //water beep
		waterBeep();
		break;
	case 1 :  //coffee beep
		coffeeBeep();
		break;
	case 2 :  //powder beep
		powderBeep();
		break;
	default : 
		break;
	}
}

void waterBeep(){
	gotoxy(ARM_TEXT_X,ARM_TEXT_Y);
	printf("beep\a\n");
}

void coffeeBeep(){
	gotoxy(ARM_TEXT_X,ARM_TEXT_Y);
	printf("BEEP~BEEP~BEEP~\a\n");
}

void powderBeep(){
	gotoxy(ARM_TEXT_X,ARM_TEXT_Y);
	printf("beep beep\a\n");
}

#endif
