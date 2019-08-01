#include <stdio.h>
#include <unistd.h>


int main(){
	int i;
	if(fork() == 0){
		sleep(1);
		printf("Sorry late\n");
		return 2;
	}
	printf("hey\n");
	wait(&i);
	printf("that what you bring me %d\n",i);
	return 0;
}
