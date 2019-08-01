#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

pthread_t *tid;

int CurThreadNum; //for covering all cased of thread that may run first

void *f1(void * tidI);

int main(int argc, char *argv[]){
	int N ,i;
	N = atoi(argv[1]);
	printf("Starts\n");
	//scanf
	tid = (pthread_t *)(malloc(sizeof(pthread_t) * N) );
	if(tid == NULL){
		printf("fail alloc\n");
		exit(1);
	}
	for(i=0;i<N;++i){
		pthread_create(tid+i,NULL,(void *)f1,(void *)(tid + i) );
	}
	sleep(1);
	free(tid);
	return 0;
}

void *f1(void * tidI){
	int taskNum =( (pthread_t *)tidI) - tid;
	struct timeval t1,t2;
	double timeDif;
	CurThreadNum = taskNum;	//covering for the first run
	gettimeofday(&t1,NULL);
	while(1){
			if(CurThreadNum != taskNum){
				gettimeofday(&t2,NULL);
				timeDif = (t2.tv_sec - t1.tv_sec) * 1000.0;
				timeDif += (t2.tv_usec - t1.tv_usec) / 1000.0;
				printf("Time slice for thread %d = %lf ms\n",taskNum+1,timeDif);	//old thread
				CurThreadNum = taskNum;
				gettimeofday(&t1,NULL);
		}
	}
}

