/* pipe3.c - send information through pipe.  */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


void sys_err(char str[]){ perror(str); exit(2);}

void child(int  pfd[]){
 	char msg[80];
char str[] = "Good Morning Parent!\n";
  
 	if (read(pfd[0], msg, 80) == -1) sys_err("read");
printf("Received  = %s\n", msg);

  	if (write(pfd[1], str, strlen(str)+1) == -1)  sys_err("write");
} 

void  main(){
 	int  pfd[2], i;
 	char msg[80];
	char str[] = "Good Morning Child!\n";

 	if(pipe(pfd) == -1)sys_err("pipe");
 	printf("pfd[0] = %d, pfd[1] = %d\n", pfd[0], pfd[1]); 
	switch(fork())  {
   		case -1:
     			sys_err("fork");
   		case 0:
     			child(pfd);
   		default:
      			break; 
 	} 

/* parent only */

  	sleep(3);

 	if (write(pfd[1], str, strlen(str)+1) == -1)   sys_err("write");

  	sleep(2);

if (read(pfd[0], msg, 80) == -1) sys_err("read");
 	printf("Received  = %s\n", msg);
}
