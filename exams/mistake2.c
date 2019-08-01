#include <stdio.h>
#include <unistd.h>

int a =0;

int main(){
	fork();
	a++;
	fork();
	a++;
	if(fork() == 0){
		printf("A\n");
	}else{
		printf("A\n");
	}
	a++;
	printf("a is %d\n",a);
}
