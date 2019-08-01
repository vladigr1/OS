#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define numOfworker 4

sem_t sem[3];

void *Cashier(){
	int num=0;
	while(1){
		sleep(1);
		printf("Cashier takes ored #%d\n",++num);
		
		sem_post(sem);
		
	}
}
void *Cook(){
	int num=0;
	while(1){
		sem_wait(sem);
		sleep(1);
		printf("Cook takes ored #%d\n",++num);
		sem_post(sem+1);
		
	}
}
void *Pack(){
	int num=0;
	while(1){
		sem_wait(sem+1);
		sleep(1);
		printf("Pack takes ored #%d\n",++num);
		sem_post(sem+2);
		
	}
}
void *Give(){
	int num=0;
	while(1){
		sem_wait(sem+2);
		sleep(1);
		printf("Give takes ored #%d\n",++num);
	}
}



int main(){
	pthread_t p_worker[numOfworker];
	int i=0, ans[numOfworker];
	for(i=0;i<numOfworker;++i){
		sem_init(sem+i,0,0);
	}
	
	ans[0] = pthread_create(p_worker, NULL, Cashier,NULL);
	ans[1] = pthread_create(p_worker, NULL, Cook,NULL);
	ans[2] = pthread_create(p_worker, NULL, Pack,NULL);
	ans[3] = pthread_create(p_worker, NULL, Give,NULL);
	sleep(10);
	return 0;
}
