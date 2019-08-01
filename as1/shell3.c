#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main(){
    char buffer[101],*word;
    int size;
    printf("shell3$**  ");
	while(1){
		size = scanf("%100[^\n]s",buffer);
		if(strcmp(buffer,"by") == 0 ) return;	//exit by typing by
		
		word = strtok(buffer," ");
		if(strcmp(word,"readFile") == 0 ) {
			word = strtok(NULL," ");
			while(word != NULL){
				if(strcmp(word,"by") == 0 ) return;
				if(fork() == 0) execlp(word,word,0);
				wait();
				word = strtok(NULL," ");
			}
		}else{
			printf("Not Supported\n");
		}
		while( getchar() != '\n');				//clear stdin
        printf("shell3$**  ");
	}
}
