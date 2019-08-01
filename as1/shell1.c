#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main(){
    char buffer[100];
    printf("shell1$**  ");
	while(scanf("%s",buffer)){
		if(strcmp(buffer,"by") == 0 ) return;
		if(strcmp(buffer,"jobs") == 0  ){
			if(fork() == 0)execlp("ps","ps",0);
		}else{
			printf("Not Supported\n");
		}
        wait();
        printf("shell1$**  ");
	}
}
