#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_ITEMS 20
#define BUF_SIZE 10
#define sem_down sem_wait
#define sem_up sem_post


pthread_mutex_t mutex;
sem_t full_count;
sem_t empty_count;


struct buffer{
	int *items;
	int size;
	int top;
}*p;

void init(struct buffer *p){
	p->items = (int*)malloc(sizeof(int)*BUF_SIZE);
	p->size = BUF_SIZE;
	p->top = 0;
}

void addProduct(struct buffer* p, int val){
	if(p->top == p->size){
		printf("Buffer is full!\n");
		return ;
	}
	p->items[p->top++] = val;
}

int consumeProduct(struct buffer *p){
	if(p->top == 0){
		printf("BUffer is empty\n");
		return -1;
	}
	return p->items[--p->top];
}

void* producer(void * args){
	int i =0;
	for(i; i<MAX_ITEMS; i++){
		sem_down(&empty_count);
		pthread_mutex_lock(&mutex);
		//critical region begins
		addProduct(p,i);
		printf("Produced product #%d\n", i);
		pthread_mutex_unlock(&mutex);
		//critical region ends
		sem_up(&full_count);
		sleep(rand()%5);
	}
	pthread_exit(0);
}

void* consumer(void * args){
	int i =0;
	for(i; i<MAX_ITEMS; i++){
		sem_down(&full_count);
		pthread_mutex_lock(&mutex);
		//critical region begins
		int product = consumeProduct(p);
		printf("Consumerd product #%d\n", product);
		//critical region ends
		pthread_mutex_unlock(&mutex);
		sem_up(&empty_count);
		sleep(rand()%10);
	}
	pthread_exit(0);
}





int main(){
	printf("Hello world\n");
	p = (struct buffer*)malloc(sizeof(struct buffer));
	init(p);
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty_count, 0, BUF_SIZE);
	sem_init(&full_count, 0, 0);

	pthread_t prod, cons;
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	printf("DONE\n");
}
