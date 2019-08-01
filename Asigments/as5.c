#include <stdio.h>
#include <dos.h>

#define NUMPRESS 5

volatile int ClockI[NUMPRESS];
volatile int numPressed;
volatile int curClock;
volatile int run;
volatile int i;

void interrupt (*old8)(void);		//time
void interrupt (*old9)(void);		//keyboard

void start();
void end();
void interrupt my9();
void interrupt my8();

void main(){
	start();
	while(run);
}

void start(){
	run =1;
	curClock =0;
	numPressed =0;
	old8 = getvect(8);
	old9 = getvect(9);
	setvect(8,my8);
	setvect(9,my9);
}

void end(){
	setvect(8,old8);
	setvect(9,old9);
	run =0;
}


void interrupt my9(){
	old9();
	numPressed++;
	if(numPressed <=10){
		if(numPressed%2 == 0){
			printf("record - index %d res %d \n",(numPressed)/2 -1,(int)(curClock/18.2));
			ClockI[numPressed/2 -1] = (int)(curClock/18.2);
			curClock =0;
		}
		if(numPressed == 10){
			for(i =0; i<5; ++i){
				printf ("%d ",ClockI[i]);
			}
			putch('\n');
			printf("Now AGAIN repeat the rhythm\n"); 
		}
	}else{
		if(numPressed%2 == 0){
			printf("repeat -index %d\n",(numPressed - 10)/2 -1);
			if((int)(curClock/18.2) == ClockI[(numPressed - 10)/2 -1]){
				printf("GOOD %d == %d in seconds\n",(int)(curClock/18.2),ClockI[(numPressed - 10)/2 -1]);
				curClock =0;
				if(numPressed == 20){
					printf("All good");
					end();
				}
			}else{
				printf("FAILED %d != %d in seconds\n",(int)(curClock/18.2),ClockI[(numPressed - 10)/2 -1]);
				end();
			}
			curClock =0;
		}
	}
}

void interrupt my8(){
	old8();
	curClock++;
}

