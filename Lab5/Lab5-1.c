#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>

char str[100];

void* func1(){
	int i;
	for(i=0;i<10;++i){
		printf("NOW str: %s\n",str);
		sleep(5);
	}
}

void* func2(void *str1){
	int i;
	for(i=0;i<30;++i){
		strncat(str,(char *)str1,3);
		sleep(1);
	}
}

void* func3(){
	int i;
	for(i=0;i<10;++i){
		strcpy(str,"X");
		sleep(7);
	}
}

int main(int argc,char *argv[]){
	pthread_t tid[3];
	int i;
	strncat(str,"ab",3);
	pthread_create(tid,NULL,func1,NULL);
	pthread_create(tid+1,NULL,func2,(void*)"cd");
	pthread_create(tid+2,NULL,func3,NULL);
	
	for(i=0;i<3;++i){
		pthread_join(tid[i],NULL);
	}
	printf("%s\n",str);
	return 0;
}
