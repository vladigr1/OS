#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sP;

void* product ()
{
    while(1){
		printf ("Product A\n");
		sem_post(&sP);
		sleep(2);
	}
}

void* collect ()
{
		while(1){
		sem_wait(&sP);
		sem_wait(&sP);
		printf ( "Collect AA\n" );
	}
}
 
int main ()
{
    pthread_t  thread[2]; 
    sem_init(&sP,0,0);
    pthread_create (thread, NULL, product, NULL);
    pthread_create (thread +1, NULL, collect, NULL );
    sleep(20);
}
