#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void main(){
    char buffer[100];
    pid_t pc;
    printf("shell$**  ");
	while(scanf("%s",buffer)){
		if(strcmp(buffer,"exit") == 0 ) return;
		if((pc = fork())  == 0) {
			if(execl(buffer,buffer,0) == -1){
              printf("Not supported\n");
              return;
            }
		}
		}else{
			waitpid(pc,NULL,0);
		}
        printf("shell$**  ");
	}
}
