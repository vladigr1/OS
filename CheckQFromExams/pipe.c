#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void f(int fd){
	dup2(fd,1);
	printf("%d ",3);
}

int main(){
	int p[2];
	int d;
	pipe(p);
	if(fork() == 0){
		f(p[1]);
		return 0;
	}
	/* //first way
	printf("read res = %d\n",read(p[0],buffer,10));
	printf("%s\n",buffer);
	*/
	dup2(p[0],0);
	scanf("%d",&d);
	printf("%d my b\n",d);
	return 0;
}
