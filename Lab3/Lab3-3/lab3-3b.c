#include <stdlib.h>


void main(int argc, char *argv[]){
	int i,n,x;
	if(argc != 2) exit(1);
	n = atoi(argv[1]);		//length
	printf("I`m the parent with pid: %d\n",getpid() );
	for(i=0;i < n; ++i){
		x = fork();
		wait(2);
		if (x == 0){		//only the parent run the for
			printf("I`m the chile %d with pid: %d\n",i+1,getpid() );
			break;
		 }
	}
	
}
