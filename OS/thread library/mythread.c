#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include <signal.h>

#define MAX_THREADS 1000
#define MEM 64000 //memory allocated to each thread
struct mythread_t{
	int id;
	ucontext_t uc;
	void *retval;
	int completed;
};


// queue for holding the contexts for round robin scheduling 
struct queue{
	int size ;
	struct mythread_t **elements;
	int front;
	int rear;
};


struct mythread_t *maint;
struct mythread_t *current;
struct queue *readyq;

struct itimerval timer;
#define INTERVAL 50000 // in milliseconds
#define algo roundrobin



void initqueue(struct queue *q){
	q->size = MAX_THREADS;
	q->elements = (struct mythread_t**)malloc(sizeof(struct mythread_t*)*q->size);
	q->front = -1;
	q->rear = -1;
	int i;
	for (i = 0; i < q->size; ++i)
	{
		q->elements[i] = NULL;
	}
}

int isEmpty(struct queue *q){
	if (q->front == -1 && q->rear == -1){
		return 1;
	}
	return 0;
}


int isFull(struct queue *q){
	if(((q->rear+1)%(q->size)) == q->front){
		return 1;
	}
	return 0;
}
int enqueue(struct queue *q, struct mythread_t *t){

	if(isFull(q)){
		printf("Queue is full\n");
		exit(0);
	}
	if(isEmpty(q)){
		q->front = 0;
		q->rear = 0;
	}
	else{
		q->rear = (q->rear+1)%q->size;	
	}
	
	q->elements[q->rear] = t;

	return q->rear;

}


struct mythread_t* dequeue(struct queue *q){
	if (isEmpty(q)){
		printf("Queue is empty\n");
		exit(1);
	}
	struct mythread_t *t = q->elements[q->front];
	// q->elements[q->front] = NULL;
	if(q->front == q->rear){
		q->front = -1;
		q->rear = -1;
	}
	else{
		q->front = (q->front +1)%q->size;
	}
	return t;
}


// queue operations over

void start_time();
void stop_time();



//schedulers----------------------------------------------------------------------------



void roundrobin(int signo){
	printf("in scheduler\n");
	stop_time();
	if(!current->completed){
		enqueue(readyq,current);
	}

	if (isEmpty(readyq)){
		current = maint;
		if(setcontext(&(maint->uc)) == -1){
			printf("set context failed\n");
			exit(1);
		}
	}


	struct mythread_t *to_run = dequeue(readyq);
	while(to_run->completed){
		to_run = dequeue(readyq);
	}
	printf("got torun %d\n", to_run->id);
	

	if(getcontext(&(current->uc)) == -1){
		printf("get context failed\n");
		exit(1);
	}
	current = to_run;
	// printf("got context\n");
	start_time();
	if(setcontext(&(to_run->uc)) == -1){
		printf("set context failed\n");
		exit(1);
	}

}

void fcfs(){
	while(!isEmpty(readyq)){

		struct mythread_t *to_run = dequeue(readyq);
		if(getcontext(&(current->uc)) == -1){
			printf("get context failed\n");
			exit(1);
		}
		current = to_run;
		printf("got to rrun\n");
		// start_time();
		if(setcontext(&(to_run->uc)) == -1){
			printf("set context failed\n");
			exit(1);
		}
	}
	exit(0);
	// current = maint;
	// printf("setting context back to main\n");
	// if(setcontext(&(maint->uc)) == -1){
	// 	printf("set context failed\n");
	// 	exit(1);
	// }
}




//timer operations

void start_time(){
	 if (setitimer(ITIMER_VIRTUAL, &timer, 0) == -1) {
	    perror("error calling setitimer() start");
	    exit(1);
	  }
}
void stop_time(){
	if (setitimer(ITIMER_VIRTUAL, 0, 0) == -1) {
	    perror("error calling setitimer() stop ");
	    exit(1);
	  }
}
void timer_init(){
	// timer = (struct itimerval)malloc(sizeof(struct itimerval));
	timer.it_value.tv_sec = INTERVAL/1000;
	timer.it_value.tv_usec = (INTERVAL*1000) % 1000000;
	timer.it_interval = timer.it_value;
	// if(signal(SIGVTALRM, fcfs) == SIG_ERR){
	// 	printf("%s\n", "Error in creating scheduler signal catcher");
	// 	exit(1);
	// }
	
}	



//timer operations done

//schedulers end---------------------------------------------------------------------------





void mythread_init(){
	maint = (struct mythread_t*)malloc(sizeof(struct mythread_t));
	
	current = maint; 
	readyq = (struct queue *)malloc(sizeof(struct queue));
	initqueue(readyq);

}

int mythread_create(struct mythread_t * thread, void *(*start_routine)(void *), void* arg){

	// (*start_routine)(arg);
	// ucontext_t thread->uc;
	if (getcontext(&(thread->uc)) == -1){
		printf("Error getting context\n");
		exit(1);
	}
	//
	thread->completed = 0;
	
	thread->id = 0;
	thread->uc.uc_link = &(maint->uc);
	thread->uc.uc_stack.ss_sp = malloc(MEM);
	thread->uc.uc_stack.ss_size = MEM;
	thread->uc.uc_stack.ss_flags = 0;
	makecontext(&(thread->uc), (void (*) ())start_routine, 1, arg);
	//making the current thread point to thread about to start running
	// current = thread;
	enqueue(readyq,thread);
	//swap context to run the thread
	// if(swapcontext(&(maint->uc), &(thread->uc )) == -1){
	// 	printf("swap context failed\n");
	// 	exit(1);
	// }
	// printf("Thread %d completed.\n", thread->id);
	return 0;
}


void mythread_join(struct mythread_t * thread){
	while(!thread->completed);
}

void mythread_exit(int retval){
	printf("exiting\n");
	// void *r = (void *) &retval;
 	// current->retval = r;
 	current->completed = 1;
 	// stop_time();
 	// start_time();
	fcfs();
}





void* test(void* arg){
	printf("This is the %d test thread\n", *(int *)arg);
	mythread_exit(1);
}

void* test2(void* arg){
	printf("second This is the %d test thread\n", *(int *)arg);
	sleep(5);
	mythread_exit(1);
}



int main(){
	mythread_init();
	timer_init();
	struct mythread_t *t1, *t2;
	t1 = (struct mythread_t*)malloc(sizeof(struct mythread_t));
	t2 = (struct mythread_t*)malloc(sizeof(struct mythread_t));
	int num = 1;
	mythread_create(t1, test, (void*)&num);
	mythread_create(t2, test2, (void*)&num);

	// start_time();

	fcfs();
	// mythread_join(t1);
	// mythread_join(&t2);
	printf("all threads completed\n");
}