
/* redirect1.c */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void fatal(char str[]){ fprintf(stderr, "%s\n", str);  exit(1);} // fatal

void sys_err(char str[]){ perror(str); exit(2);} // sys_err

int main(){

   if (fork() == 0) { /* Child */
int fd1, fd2;

  	if ( (fd1 = open("hello.txt", O_WRONLY | O_CREAT |O_TRUNC, 0644)) == -1)
sys_err("open");
if( close(1) == -1)   sys_err("close"); //סגירת ערוץ הפלט הסטנדרטי

   	fd2 = dup(fd1);  //ניתוב פלט סטנדרטי (1) לקובץ 
   	if ( fd2 == -1 )  sys_err("dup");
   	else
   		if (fd2 != 1)      fatal("Unexpected dup result"); 
     
    	execl("./hello", "hello", 0); // כרגע הפלט יופיע בקובץ במקום על מסך
    } /* if  fork*/

    printf("Parent terminating\n");
} /* main */
