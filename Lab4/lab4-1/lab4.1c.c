/* Lab4_4.c */

#include <stdio.h>
#include<unistd.h>

void main(){
    int id;
    printf("Here comes the date:\n");
    if ( ( id =  fork() ) == 0 )  {  
       printf("PID is %d and ID is %d.\n", getpid(), id);
       execl("/bin/date", "date",0);
  }
    printf("PID is %d and ID is %d.\n", getpid(), id);
    printf("That was the date.\n");
}
