#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include <signal.h>

#define MAX_THREADS 10
#define MEM 64000 //memory allocated to each thread
struct mythread_t{
	int id;
	ucontext_t uc;
	void *retval;
};


// queue for holding the contexts for round robin scheduling 
struct queue{
	int size ;
	struct mythread_t *elements;
	int front;
	int rear;
};

void initqueue(struct queue q){
	q.size = MAX_THREADS;
	q.elements = (struct mythread_t*)malloc(sizeof(struct mythread_t)*q.size);
	q.front = -1;
	q.rear = -1;
}

int isEmpty(struct queue q){
	if (q.front == -1 && q.rear == -1){
		return 1;
	}
	return 0;
}


int isFull(struct queue q){
	if((q.rear+1)%q.size == q.front){
		return 1;
	}
	return 0;
}
int enqueue(struct queue q, struct mythread_t t){
	if(isFull(q)){
		printf("Queue is full\n");
		return;
	}
	if(isEmpty(q)){
		q.front = 0;

	}
	q.rear = (q.rear+1)%q.size;	
	q.elements[q.rear] = t;
	return q.rear;

}


struct mythread_t dequeue(struct queue q){
	if (isEmpty(q)){
		printf("Queue is empty\n");
		exit(1);
	}
	struct mythread_t t = q.elements[q.front];
	// q.elements[q.front] = NULL;
	if(q.front = q.rear){
		q.front = -1;
		q.rear = -1;
	}
	else{
		q.front = (q.front +1)%q.size;
	}
	return t;
}


// queue operations over



struct mythread_t *maint;
struct mythread_t *current;
struct queue readyq;


void mythread_init(){
	maint = (struct mythread_t*)malloc(sizeof(struct mythread_t));
	if (getcontext(&(maint->uc)) == -1){
		printf("Error getting context\n");
		exit(1);
	}
	current = maint; 

}

int mythread_create(struct mythread_t * thread, void *(*start_routine)(void *), void* arg){

	// (*start_routine)(arg);
	// ucontext_t thread->uc;
	if (getcontext(&(thread->uc)) == -1){
		printf("Error getting context\n");
		exit(1);
	}
	//
	// thread->id = enqueue(readyq,*(thread));
	thread->id = rand()%100;
	thread->uc.uc_link = &(maint->uc);
	thread->uc.uc_stack.ss_sp = malloc(MEM);
	thread->uc.uc_stack.ss_size = MEM;
	thread->uc.uc_stack.ss_flags = 0;
	makecontext(&(thread->uc), (void (*) ())start_routine, 1, (void *)&thread->id);
	//making the current thread point to thread about to start running
	current = thread;
	//swap context to run the thread
	if(swapcontext(&(maint->uc), &(thread->uc )) == -1){
		printf("swap context failed\n");
	}
	printf("Thread %d completed.\n", thread->id);
	return 0;
}



void mythread_exit(int retval){
	void *r = (void *) &retval;
 	current->retval = r;

}





void* test(void* arg){
	printf("This is the %d test thread\n", *(int *)arg);
	mythread_exit(1);
}

void* test2(void* arg){
	printf("This is the %d test thread\n", *(int *)arg);
	sleep(5);
	mythread_exit(1);
}



int main(){
	mythread_init();
	struct mythread_t t1, t2;
	mythread_create(&t1, test, 0);
	mythread_create(&t2, test2, 0);
	printf("all threads completed\n");
}