/* Lab4_2.c - call system date routine using execl */

#include <stdio.h>
#include<unistd.h>

void main(){
    printf("Here comes the date:\n");
    execl("/bin/date", "date", 0);
    printf("ERROR - execl failed.\n");
}
