#include <dos.h>
#include <stdio.h>

#define N 10
void interrupt (*oldISR9)(void);

volatile int count;

void interrupt myISR9(void){
	oldISR9();
	count++;
}

void main(){
	oldISR9 = getvect(9);
	setvect(9,myISR9);		//destroy
	count = 0;
	while(count<N);
	printf("END\n");
	setvect(9,oldISR9);		//rebuild
}