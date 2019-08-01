#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "./flights.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

/*
 * flight.c
 *
 *  Created on: May 1, 2019
 *      Authors: Sivan Biton Vlad Barkanass
 */

// check if free: infoNode - freed imiditly in order,faillist and moreThenOne free while printing 

typedef struct{
	int id,flight,seats;
}info;

//list of orders that there arent enough space on the plain
typedef struct node{
	int id,flight,seats;
	struct node *next;
}infoNode;

//list of orders that have more then one seats
typedef struct mnode{
	int id,flight,seats;
	int *requested;
	struct mnode *next;
}moreThenOneNode;

infoNode *failList = NULL;
moreThenOneNode *moreThenOneSeatList= NULL;

int map[FLIGHT_COUNT][ROW_COUNT][SEATS_PER_ROW] = {0};

//readMutex:pulling request and order use a pointer of info we dont start new pull if the cur info didnt readed by order
//sem_t readMutex;	//is not need because we allocate a uniqe address for info type so we dont really need it it changes every order
//I mean the adress of the info change every itertaion of the main thread so their cant be over crose info


//mutexSetSeats:only one thread can check if his seats are taken
sem_t mutexSetSeats;
int emptySeats[FLIGHT_COUNT];
//mutexEmptySeats only one thread update empty seats for it to be valid
sem_t mutexEmptySeats;
//mutexOrderCounter Only one thread will grabe the info and change orderCounter and Counter
sem_t mutexOrderCounter;
int orderCounter = 0;


void *order(void * order);
info *pullRequest(FILE * file);
int *requestFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight,int size);
int checkFlightrequest(int map[][ROW_COUNT][SEATS_PER_ROW],int flight,int *requested,int size,int id);
void printFail();
void printMoreThenOne();
void printFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight);
void PrintEmptyFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight);

int main(int argc,char *argv[]){
	FILE *file;
	info *cur;
	pthread_t  *threads;
	int i, check =0, rows =0;
	char c;
	struct timeval t1,t2;
	double timeDif;
	if(argc < 2){
		printf("Please enter requests.txt path as main paramater\n(./requests.txt)\n");
		exit(1);
	}
	
	gettimeofday(&t1,NULL);		//the progarm start running
	
	srand(time(NULL));
	file = fopen(argv[1],"r");
	sem_init(&mutexOrderCounter,0,1);
	sem_init(&mutexEmptySeats,0,1);
	sem_init(&mutexSetSeats,0,1);
	//sem_init(&readMutex,0,1);
	
	if(file == NULL){
		printf("file openning error\n");
		exit(1);
	}

	 // Extract characters from file and store in character c 
    for (c = getc(file); c != EOF; c = getc(file)) 
    {    if (c == '\n') // Increment count if this character is newline 
            rows++;
	}
	fseek(file, 0, SEEK_SET);

	threads = (pthread_t *)malloc(rows*sizeof(pthread_t));

	for(i = 0;i< FLIGHT_COUNT; ++i)
	{	emptySeats[i] = ROW_COUNT*SEATS_PER_ROW;	//setting empty seats in flight
	}
	// read requests in order
	i=0;
	do{		
		if(i>rows) break;			
		//sem_wait(&readMutex);		
		check++;		
		cur = pullRequest(file);
		if(cur == NULL) break;	
		if(pthread_create(&threads[i],NULL,order,(void *)cur) != 0){ // cur changes after iteration
			printf("couldn't create thread\n");
			exit(1);
		}
		i++;
	}while(1);
	rows = i;
	for(i=0;i<rows;i++)	pthread_join(threads[i], NULL);
	fclose(file);
	free(threads);
	
	//report
	printFail();	
	for(i=0;i<FLIGHT_COUNT;++i){
		//printFlightSeat(map,i);
		PrintEmptyFlightSeat(map,i);
	}
	printMoreThenOne();
	
	
	gettimeofday(&t2,NULL);
	timeDif = (t2.tv_sec - t1.tv_sec) * 1000.0;
	timeDif += (t2.tv_usec - t1.tv_usec) / 1000.0;
	printf("Time took to finish the program %lfms\n",timeDif);
	
	return 0;
}


info* pullRequest(FILE * file){
	info *readed;
	int id,flight,seats,rbytes;
	id = flight = seats = -1;
	
	if(feof(file)){
		//printf("End of file\n");
		return NULL;
	}

	rbytes = fscanf(file,"%d %d %d\n",&id,&flight,&seats);
	
	if(rbytes == -1){
		printf("file reading  error\n");
		exit(1);
	}
	if(id == -1 || flight == -1 || seats == -1){
		printf("reading variables error\n");
		exit(1);
	}
	readed = (info *)malloc(sizeof(info));
	if(readed == NULL){
		printf("malloc error\n");
		exit(1);
	}
	//printf("in pull: %d %d %d\n",id,flight,seats);
	readed->id = id;
	readed->flight = flight;
	readed->seats = seats;
	return readed;
}

void *order(void * order){
	int id,flight,seats;
	id = flight = seats = 0;
	int *requested;
	sem_wait(&mutexOrderCounter);
	id = ((info *)order)->id;
	flight= ((info *)order)->flight;
	seats= ((info *)order)->seats;
	free(order);
	orderCounter++;
	printf("orders in the system: %d|first time enter to order with info id:flight:seats %d:%d:%d\n",orderCounter,id,flight,seats);
	//sem_post(&readMutex);
	sem_post(&mutexOrderCounter);
	while(1){
		sem_wait(&mutexEmptySeats);
		if(emptySeats[flight] < seats){
			int curEmpty = emptySeats[flight];
			sem_post(&mutexEmptySeats);	//most up the mutex and its not needed for this part of code
			infoNode *failNode = (infoNode *)malloc(sizeof(infoNode));
			if(failNode == NULL){
				printf("malloc error\n");
				exit(1);
			}
			failList->id = id;
			failNode->flight = flight;
			failList->seats = seats;
			failNode->next = failList;
			failList = failNode;
			//didnt free order of failed because we need it thats why i cant break
			sem_wait(&mutexOrderCounter);
			printf("orders in the system: %d|order failed requestd: %d empty: %d for id: %d\n",orderCounter,seats,curEmpty,id);
			orderCounter--;
			sem_post(&mutexOrderCounter);
			return NULL;
		}//there are enough empty seats
		sem_post(&mutexEmptySeats);
		
		printf("orders in the system: %d|request order for id: %d\n",orderCounter,id);
		requested = requestFlightSeat(map,flight,seats);
		
		usleep(rand()%20);
		printf("orders in the system: %d|check order for id: %d\n",orderCounter,id);
		if(checkFlightrequest(map,flight,requested,seats, id) == 1){
			if(seats >1){
				moreThenOneNode *moreNode = (moreThenOneNode *)malloc(sizeof(moreThenOneNode));
				if(moreNode == NULL){
					printf("malloc error\n");
					exit(1);
				}
				moreNode->id = id;
				moreNode->flight = flight;
				moreNode->seats = seats;
				moreNode->requested = requested;
				moreNode->next = moreThenOneSeatList;
				moreThenOneSeatList = moreNode;
			}else{// the request is for one sit so i cant free the request and the order
				free(requested);
			}
			//sit register
			break;
		}
		//printf("orders found out Seats are taken\n"); the place where order when seats are taken
		//else places where cought make new request can free request but not the order need to order new sits
		free(requested);
	}
	usleep(100);
	sem_wait(&mutexOrderCounter);
	orderCounter--;
	printf("orders in the system: %d|finished order for id: %d\n",orderCounter,id);
	sem_post(&mutexOrderCounter);
	return NULL;
}

int *requestFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight,int size){
	int *requsted =(int *) malloc(sizeof(int)*size),curEnter = 0;
	int i, j;
	if(requsted == NULL){
		printf("malloc error\n");
		exit(1);
	}
	for(i=0;i<ROW_COUNT;++i){
		for(j=0;j<SEATS_PER_ROW;++j){
			if( map[flight][i][j] == 0 ){
				*(requsted + curEnter++) = i*SEATS_PER_ROW +j;	//easy way to translate row and sit in one number
			}
			if(curEnter == size)return requsted;
		}
	}
	//printf("\n\nmiss request\n\n");
	return NULL;
}

int checkFlightrequest(int map[][ROW_COUNT][SEATS_PER_ROW],int flight,int *requested,int size,int id){
	sem_wait(&mutexSetSeats);
	int i;
	for(i = 0;i < size;++i){
		printf("requested:(r,s) = (%d,%d)\n",requested[i]/SEATS_PER_ROW,requested[i]%SEATS_PER_ROW);
		if(map[flight][requested[i]/SEATS_PER_ROW][requested[i]%SEATS_PER_ROW] != 0 ){
			//bad request
			printf("\n\n\t\trequest has been taken\n\n");
			sem_post(&mutexSetSeats);
			return 0;
		}
	}
	sem_wait(&mutexEmptySeats);
	emptySeats[flight] -= size;
	sem_post(&mutexEmptySeats);
	//good request
	for(i = 0;i < size;++i){
		map[flight][requested[i]/SEATS_PER_ROW][requested[i]%SEATS_PER_ROW] =id;
	}
	sem_post(&mutexSetSeats);
	return 1;
}

void printFail(){
	infoNode *cur = failList;
	int amount=0;
	while(cur != NULL){
		infoNode *temp = cur;
		amount++;
		printf("failed order id: %d flight: %d number of seats: %d \n",cur->id,cur->flight,cur->seats);
		cur = cur->next;
		free(temp);
	}
	printf("The total amount of people whose order have failed %d\n",amount);
}

void printMoreThenOne(){
	moreThenOneNode *cur = moreThenOneSeatList;
	int i,amount =0;
	while(cur != NULL){
		amount++;
		moreThenOneNode *temp = cur;
		printf("order that has more then one seat id: %d flight: %d number of seats: %d\nseats:\n(r,s) = ",cur->id,cur->flight,cur->seats);
		for(i =0 ; i < cur->seats;++i){
			printf("(%d,%d) ",cur->requested[i]/SEATS_PER_ROW,cur->requested[i]%SEATS_PER_ROW);
		}
		printf("\n");
		cur = cur->next;
		free(temp->requested);
		free(temp);
	}
	printf("The total amount of people who order more then one seats are %d\n",amount);
}

void PrintEmptyFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight){
	int i,j,sum=0,newLineFlag;
	printf("flight empty seats%d:\n",flight);
	for(i=0;i<ROW_COUNT;++i){
		newLineFlag =0;
		for(j=0;j<SEATS_PER_ROW;++j){
			if(map[flight][i][j] == 0){
				newLineFlag =1;
				printf("(%d,%d) ",i,j);
				sum++;
			}
		}
		if(newLineFlag == 1)printf("\n");
	}
	printf("flight %d has %d free seats\n\n",flight,sum);
}

void printFlightSeat(int map[][ROW_COUNT][SEATS_PER_ROW],int flight){
	int i,j;
	printf("\t\tFlight %d:\n",flight);
	for(i=0;i<ROW_COUNT;++i){
		for(j=0;j<SEATS_PER_ROW;++j){
			printf("%d ",map[flight][i][j]);
		}
		printf("\n");
	}
}

