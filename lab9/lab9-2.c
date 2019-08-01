#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define numOfCats 3
#define numOfServs 3


//
//ask: is it okay to put in critic code wait 
//
int servings;
sem_t fill,empty,catEnter;

void printServing(){
	printf("Serveings = %d.\n",servings);
}

void getServingFromPot(int i){
	printf("\nI'm a Cat N#%d.",i);
}
void eat(){
	printf("I`m eating. \n");
}

void putServingsInPot(){
	printf("\nI'm a MAN. I put the food.");
}

void *Cat(void * i){
	while(1){
		sem_wait(&catEnter);
		if(servings <= 0){
			sem_post(&fill);
			sem_wait(&empty);
		}
		getServingFromPot(*(int *)i);
		eat();
		printServing();
		sleep(1);
		servings--;
		sem_post(&catEnter);
		sleep(1);
	}
	
}
void *Man(){
	while(1){
		sem_wait(&fill);
		putServingsInPot();
		servings = numOfServs;
		sem_post(&empty);
	}
}

int main(){
	pthread_t cats[numOfCats],hman;
	int index[numOfCats],i;
	sem_init(&fill,0,0);
	sem_init(&empty,0,0);
	sem_init(&catEnter,0,1);
	pthread_create(&hman,NULL,Man,NULL);
	for(i=0 ;i< numOfCats;++i){
		index[i] = i;
		pthread_create(cats+i,NULL,Cat,index + i);
	}
	sleep(10);
	return 0;
}
