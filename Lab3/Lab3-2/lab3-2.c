#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


void main(int argc,char *argv[]){
	char buffer[101],*token;
	int fd1,fd2,l;
	if(argc <3) exit(1);
	fd1 = open(argv[1],O_RDONLY,S_IRUSR);
	fd2 = open(argv[2],O_WRONLY,S_IWUSR);
	if(fd1 == -1 || fd2 == -1)exit(1);
	while( (l = read(fd1,buffer,100) ) > 0){
		buffer[l] = '\0';
		token = strtok(buffer, " ");
		while(token != NULL){
			write(fd2,token,strlen(token));
			write(fd2,"\n",1);
			token = strtok(NULL, " ");
		}
	}
	write(fd2,"\0",1);
	close(fd1);
	close(fd2);
}
