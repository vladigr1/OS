#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

sem_t sM, sT;

void* create_message ( void* str )
{
    int i = 0;
    for ( i = 0; i < 10; i++ ) {
		sem_wait(&sT);
		printf ( "I've wrote a message #%d. %s\n", i+1, (char*) str );
		sem_post(&sM);
	}
}
 
int main ()
{
    pthread_t  thread; 
    sem_init(&sM,0,0);
    sem_init(&sT,0,1);
    int i = 0;
    pthread_create (&thread, NULL, create_message, (void*)"Thread A" );
    for ( i = 0; i < 10; i++ ) {
		sem_wait(&sM);
		printf ( "The message #%d was printed. Thread B \n", i+1 );
		sem_post(&sT);
	}
    pthread_join(thread, NULL);
}
