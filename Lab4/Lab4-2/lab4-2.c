#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(){
    if (fork() == 0 )  {  
        execl("./t1.out","t1.out",NULL);
    }
    wait();
    if (fork() == 0 )  {  
        execl("./t2.out","t2.out",NULL);
    }
    wait();
    printf("END\n");
}
