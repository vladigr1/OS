#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

long int *counters;

void f1(void * counterI);

void fPrint(void *pN);

int main(int argc, char *argv[]){
	int N ,i;
	pthread_t *tid;						//ask sheli what a solution she recommand for not needing the tid
	N = atoi(argv[1]);
	tid = (pthread_t *)(malloc(sizeof(pthread_t) * (N + 1) ) );
	counters = (long int *)malloc(sizeof(long int) * (N + 1) );
	if(tid == NULL || counters == NULL){
		printf("fail alloc\n");
		exit(1);
	}
	for(i=0;i<N;++i){
		counters[i] = 0;
	}
	for(i=0;i<N;++i){
		pthread_create(tid+i,NULL,f1,(void *)(counters + i) );
	}
	pthread_create(tid+i,NULL,fPrint,&N);
	sleep(20);
	free(tid);
	free(counters);
	return 0;
}

void f1(void * counterI){
	while(1){
		++(*( (int *) counterI));
		//sleep(2);
	}
}

void fPrint(void *N){
	int i;
	while(1){
		for(i=0;i< *(int *)N;++i){
			printf("counters[%d] = %ld\n",i,counters[i]);
		}
		sleep(2);
	}
}
