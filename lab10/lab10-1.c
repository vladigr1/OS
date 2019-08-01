#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

sem_t citys[4];
int pindex =  -1;
sem_t priority;
sem_t mutex1;

void Calls(int city1,int city2);
void *Func(void *city);

int main(){
	int i,index[4];
	pthread_t t[4];
	srand(time(NULL));
	for(i=0;i<4;++i){
		sem_init(citys+i,0,1);
	}
	sem_init(&priority,0,1);
	sem_init(&mutex1,0,1);
	sem_init(&mutex2,0,1);
	for(i=0;i<4;++i){
		index[i] =i;
		pthread_create(t+i,NULL,Func,(void *)(index + i));
	}
	sleep(20);
	return 0;
}

void Calls(int city1,int city2){
	sem_wait(&mutex1);
	sem_wait(citys + city1);
	sem_wait(citys + city2);
	if((city1 == 2 || city1 == 3 ) && (city2 == 2 || city2 == 3 )){	//cant enter if they both call
	}else{
		sem_post(&mutex1);
	}
	printf("%d started to talk with %d\n",city1,city2);
	sleep(1);
	printf("%d finished to talk with %d\n",city1,city2);
	sem_post(citys + city1);
	sem_post(citys + city2);
	if((city1 == 2 || city1 == 3 ) && (city2 == 2 || city2 == 3 )){
		sem_post(&mutex1);
	}
}

void *Func(void *city){
	while(1){
		int r,c = *(int *)city;
		do{
			r = rand()%4;
		}while(r == c);
		Calls(c,r);
		sleep(1);
	}
	return NULL;
}
