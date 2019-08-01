#include <stdio.h>

void main(){
	int i=1,N=10;
	while(i<=N){
		if(fork() == 0){
            sleep(1);
            printf("son: %d\n",i);
			i++;
		}else{
            sleep(1);
            wait();
			printf("father: %d\n",i);
			break;
		}
	}
}
