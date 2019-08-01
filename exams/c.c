#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define HELLO_N 5
int main(){
	int i, j, fd;
	pid_t children[HELLO_N];
	close(1);fd = open("stam.txt", O_RDWR|O_CREAT, 0644);
	dup(fd);
	for(i=0; i<HELLO_N;i++) {
		pid_t pid = fork() ;
		switch(pid){
			case -1:
			break;
			case 0:
			break;
			default:children[i] = pid;
		}
	}
	for(j=0; j<i; j++){
		waitpid(children[j], NULL,0);
	}
	printf("End\n");
	return 0;
	}

