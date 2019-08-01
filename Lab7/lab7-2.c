#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define N 10

int toRun;

void *f1(void *param);

int main(){
	int i ,indexs[N];
	pthread_t tid[N];
	toRun = 0;
	for(i=0;i<N;++i){
		indexs[i] = i;
		pthread_create(tid + i,NULL,f1,indexs + i);
	}
	for(i=0;i<N;++i){
		pthread_join(tid[i],NULL);
	}
	printf("\n");
}

void *f1(void *param){
	while(1){
		if(toRun == (*(int *)param) ){
			printf("%d ",*(int *)param);
			toRun++;
			break;
		}
	}
}
