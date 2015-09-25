/* Solution to dining philosophers problem by resource heirarchy method.
# Layout of the table (P = philosopher, f = fork):
#          P0
#       f3    f0
#     P3        P1
#       f2    f1
#          P2

So , a philosopher has to pick up the fork with the lower index first! so if four out of the five philosophers pick 
up the smaller fork, there will be one philosopher left who won't be able to acquire the smaller fork! 
This prevents the deadlock condition where all philosophers pick up their left fork and wait indefinitely.
# Number of philosophers at the table. There'll be the same number of forks.


LEft fork will be same index as philosopher
RIght fork will be one index less.
*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define NUMBEROFPHILOSOPHERS 4
#define TIMES 5

struct philosopher{
	int index;
	int first_fork;
	int second_fork;
}*p;


pthread_mutex_t f[NUMBEROFPHILOSOPHERS];


void philosopher_init(struct philosopher *p, int i){
	p->index = i;
	int leftfork = i;
	int rightfork;
	if(p->index == 0)
		rightfork = NUMBEROFPHILOSOPHERS-1;
	else
		rightfork = i-1;

	//now choosing the first fork as the smaller indexed one
	if(leftfork<rightfork){
		p->first_fork = leftfork;
		p->second_fork = rightfork;
	}
	else{
		p->first_fork = rightfork;
		p->second_fork = leftfork;
	}

	printf("philospher %d's first_fork is %d, second_fork is %d\n", p->index, p->first_fork,p->second_fork);
}


void acquire_first_fork(struct philosopher *phil){
	pthread_mutex_lock(&f[phil->first_fork]);
}
void acquire_second_fork(struct philosopher *phil){
	pthread_mutex_lock(&f[phil->second_fork]);
}

void release_first_fork(struct philosopher *phil){
	pthread_mutex_unlock(&f[phil->first_fork]);
}
void release_second_fork(struct philosopher *phil){
	pthread_mutex_unlock(&f[phil->second_fork]);
}



void* philosopher(void *arg){
	struct philosopher *phil = ((struct philosopher*)arg);
	if(phil == NULL){
		printf("NULL pointer\n");
	}
	int i;
	for (i = 0; i < TIMES; ++i)
	{
		acquire_first_fork(phil);
		acquire_second_fork(phil);
		printf("philosopher #%d is eating.\n", phil->index);
		sleep(rand()%3);
		release_second_fork(phil);
		release_first_fork(phil);

	}
	pthread_exit(0);


}
int main(){
	pthread_t phil_threads[NUMBEROFPHILOSOPHERS];
	int i;
	for (i = 0; i < NUMBEROFPHILOSOPHERS; ++i)
	{
		pthread_mutex_init(&f[i],NULL);
	}

	for (i = 0; i < NUMBEROFPHILOSOPHERS; ++i)
	{
		struct philosopher *phil = (struct philosopher *)malloc(sizeof(struct philosopher));
		philosopher_init(phil,i);
		pthread_create(&phil_threads[i], NULL, philosopher, (void*) phil);
	}


	for (i = 0; i < NUMBEROFPHILOSOPHERS; ++i)
	{
		pthread_join(phil_threads[i],NULL);
	}

}
