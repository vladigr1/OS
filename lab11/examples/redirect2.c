#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void sys_err(char str[]){	perror(str); 	exit(2);} 

int main(){
 if (fork() == 0) {
   		int fd1, fd2;

   		if ( (fd1 = open("hello.txt", O_WRONLY | O_CREAT |O_TRUNC, 0644)) == -1)    			sys_err("open");

   		fd2 = dup2(fd1, 1);
   		if ( fd2 == -1 )      sys_err("dup");
     
    		execl("./hello", "hello", 0);

  	} /* if fork */

 	printf("Parent terminating\n");} 
