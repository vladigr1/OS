/* pipe1.c - send information through pipe.  */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void sys_err(char str[]){ 
	perror(str); 
	exit(2);
}

void child(int  pfd[]){
  	int i;
  	char msg[80];
  	/*
  הבן סוגר את ערוץ הפלט
  */
  	if (close(pfd[1]) == -1)
       		sys_err("close");

 	i = 0;
 	/*
וקורא מערוץ הקלט תו אחרי תו שקודם רשם לשם האב
*/
  	do {
     		if (read(pfd[0], &msg[i], 1) == -1)
sys_err("read");
      		i++;
   	} while (msg[i-1] != '\0');      

  	printf("Received  = %s\n", msg);
} 

int main(){
 	int  pfd[2], i;
char str[] = "Hello World!\n";
  	
if (pipe(pfd) == -1)  sys_err("pipe");
  	printf("pfd[0] = %d, pfd[1] = %d\n", pfd[0], pfd[1]); 
switch(fork()) {
	case -1:
	sys_err("fork");
case 0:
     		child(pfd);
   	default:
    			break; 
} 

/* parent only */
/*האב סוגר את ערוץ הקלט*/ 
 	if (close(pfd[0]) == -1)  sys_err("close");
 
 	sleep(3);
 	/*
ורושם (פולט) לערוץ הפלט תו אחרי תו
* */
for(i=0; i <= strlen(str); i++)
	if (write(pfd[1], &str[i], 1) == -1)  sys_err("write");
   
   wait();
return 0;
}
