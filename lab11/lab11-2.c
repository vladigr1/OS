#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



int main(){
	int pfd[2],readed,chars,words,lines;
	char buffer[100] = {0};
	readed = chars = words = lines = 0;
	pipe(pfd);
	switch(fork()){
		case -1:
			printf("fork error\n");
			exit(1);
		case 0:
			close(pfd[0]);
			dup2(pfd[1],1);
			execlp("date","date",0);
			exit(1);
		default:
		break;
	}
	do {
		readed = read(pfd[0], buffer + chars, 1);
		if (readed == -1){
			printf("read error\n");
		}
		if(buffer[chars] == ' '){
			words++;			
		}
		if(buffer[chars] == '\n'){
			lines++;
			break;
		}
		chars++;
   	} while (1);  
   	buffer[chars+1] = 0;
	close(pfd[0]);
	close(pfd[1]);
	printf("%s",buffer);
	printf("Chars\tWords\tLines:\n%d \t %d\t %d\n",chars,words,lines);
	return 0;
}
