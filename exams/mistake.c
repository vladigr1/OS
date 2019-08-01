#include <stdio.h>
#include <unistd.h>

int main(){
	int pid = fork();
	if(pid !=0){
		fork();
	}
	if(pid != 0){
		printf("A\n");
	}else{
		printf("B\n");
	}
}
