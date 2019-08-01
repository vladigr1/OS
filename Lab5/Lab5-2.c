#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define N 1000
#define K 4

int arr[N];

typedef struct{	//with the ends
	int start;
	int end;
	//int *pres;	//good programming you return the value by parmater which mean you dont cast it on pointer size;
}param;

void *sumFT(void *argCur);

int main(int argc, char *argv[])
{
	int i,sum = 0, fres = 0,cres;
	param params[K];
	pthread_t tid[K];
	srand(time(NULL));
	for(i=0;i<N;++i){
		arr[i] = rand()%N;
	}

	for(i=0;i<K-1;++i){
		params[i].start = i*(N/K);
		params[i].end = (i+1)*(N/K) -1;	//not doing it twice
		pthread_create(tid+i,NULL,sumFT,params+i);
	}
	
		params[i].start = i*(N/K);		//i = k-1
		params[i].end = N-1;
		pthread_create(tid+i,NULL,sumFT,params+i);
		
	for(i=0;i<N;++i){		//sum with out threads
		//printf("%d ",arr[i]);
		sum += arr[i];
	}
	
	for(i=0;i<K;++i){		//calc sum by threads
		pthread_join(tid[i],(void **)&cres);d 
			//bad programming you send int us and int and not pointer 
		//printf("\ncres = %d\n",cres);
		fres += cres;
	}

	printf("\n total sum = %d and the result by threads %d\n",sum,fres);
	return 0;
}

void *sumFT(void *argCur){
	int i,sum =0;
	param *cur = argCur;
	for(i=cur->start; i <= cur->end;++i){
		sum += arr[i];
	}
	return (void *)sum;									//ask about it
}
