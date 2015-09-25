/*
Refer to wiki page for sleeping barber.
Implementing the solution by using semaphores and mutexes.
*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define sem_down sem_wait
#define sem_up sem_post


pthread_mutex_t barberReady, accessSeats;
sem_t custReady;

int freeSeats = 5;


void* barber(void * arg){
	while(1){
		sem_down(&custReady); // barber is waiting for a customer
		pthread_mutex_lock(&accessSeats);
		freeSeats++;
		pthread_mutex_unlock(&barberReady); 
		// barber has decided to cut the particular customers hair so he declares ready to cut.
		// the above statement has to be placed inside the critical section of the accessSeats mutex becasue otherwise
		// another customer who has just come in can down barberReady 
		pthread_mutex_unlock(&accessSeats);
		printf("Barber is cutting hair\n");
		sleep(3);

	}
}

void* customer(void *arg){
	pthread_mutex_lock(&accessSeats);
	if (freeSeats > 0){
		freeSeats--;
		printf("Customer Id %d Just sat down. free seats are %d \n", (int)pthread_self(), freeSeats);
		sem_up(&custReady);
		pthread_mutex_unlock(&accessSeats);
		pthread_mutex_lock(&barberReady); // wait until barber is ready

	}
	else{
		printf("Customer id %d just left the shop\n", (int)pthread_self());
		pthread_mutex_unlock(&accessSeats);
	}


}

int main(){
	pthread_t b, c[10];
	pthread_mutex_init(&barberReady,NULL);
	pthread_mutex_init(&accessSeats,NULL);
	sem_init(&custReady, 0, 0);

	pthread_create(&b, NULL,barber,NULL);
	int i;
	for (i = 0; i < 10; ++i)
	{
		pthread_create(&c[i],NULL,customer,NULL);
		sleep(rand()%3);
	}
	pthread_join(b,NULL);
	for (i = 0; i < 10; ++i)
	{
		pthread_join(c[i],NULL);
	}
	printf("DONE\n");
}
