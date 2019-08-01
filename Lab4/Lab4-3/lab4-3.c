#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main(){
    char buffer[100];
    printf("$");
	while(scanf("%s",buffer)){
		if(strcmp(buffer,"q") == 0 ) return;
		if(fork() == 0) {
			execlp(buffer,buffer,0);
		}
        wait();
        printf("\n$");
	}
}
