#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main(){
    char buffer[100];
    printf("shell2$**  ");
	while(scanf("%s",buffer)){
		if(strcmp(buffer,"by") == 0 ) return;
		if(strcmp(buffer,"mydate") == 0 ) {
			if(fork() == 0)execlp("date","date",0);
		}else{
			printf("Not Supported\n");
		}
        wait();
        printf("shell2$**  ");
	}
}
