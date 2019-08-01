#include <stdlib.h>


void main(int argc, char *argv[]){
	int i,n,x,ppid,cppid;
	if(argc != 2) exit(1);
	n = atoi(argv[1]);		//length
	for(i=0;i<n;++i){
		ppid = getpid();
		x=fork();
		if(x!= 0){
			break;
		}else{
			cppid = ppid;	//save next parent because it will not enter while printing
		}
	}
	if( i == 0){
		printf("I`m the parent %d\n",getpid());
	}else{
		printf("I`m the child of pid %d and my pid %d\n",cppid,getpid());
	}
	wait(2);
}
