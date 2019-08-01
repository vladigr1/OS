#include <stdio.h>
#include <unistd.h>

#define N 10

int main(){
	int c1,c2,t,i;
	c2 =1;
	c1=0;
	for(i=1;i<N;i++){
		switch(fork()){
			case -1:
				printf("Error creating process\n");
				return -1;
				
			case 0:
				c2 +=c1;
				c1 = c2 - c1;
				break;
			
			default:
			i = N;
			break;
		}
	}
	wait();
	printf("%d ",c2);
}
