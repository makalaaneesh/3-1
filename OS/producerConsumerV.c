#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#define MAX_ITEMS 20
#define BUF_SIZE 10



/*-----------------------------Helper Functions -------------------------------------*/


int AllocateSharedMemory(int n){

	return shmget(IPC_PRIVATE, n, IPC_CREAT |0666);
}

void* MapSharedMemory(int id){
	void *addr;
	addr = shmat(id,NULL,0); // id, specify NULL so the system choses the space to map the memory, flags are usually 0
	shmctl(id, IPC_RMID, NULL); // set the memory to be destroyed after the last process detaches it.
	return addr;
}


void sem_down(int semid){
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = 0;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s\n", "Error locking semaphore");
	}
}
void sem_up(int semid){
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = 0;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s\n", "Error unlocking semaphore");
	}
}


/*--------------------------------------------------------------------------------------*/




int mutex;
int full_count;
int empty_count;




struct buffer{
	int items[BUF_SIZE];
	int size;
	int top;
}*p;

void init(struct buffer *p){
	// p->items = (int*)malloc(sizeof(int)*BUF_SIZE);
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

void print(struct buffer *p){
	int i = 0;
	while(i!= p->top)
	{
		printf("%d,", p->items[i]);
		i++;
	}
		printf("%s\n", "");
}
void producer(){
	int i =0;
	for(i; i<MAX_ITEMS; i++){
		sem_down(empty_count);
		sem_down(mutex);
		//critical region begins
		addProduct(p,i);
		printf("Produced product #%d\n", i);
		print(p);
		sem_up(mutex);
		//critical region ends
		sem_up(full_count);
		sleep(rand()%5);
	}
}

void consumer(){
	int i =0;
	for(i; i<MAX_ITEMS; i++){
		sem_down(full_count);
		sem_down(mutex);
		//critical region begins
		int product = consumeProduct(p);
		printf("Consumed product #%d\n", product);
		print(p);
		//critical region ends
		sem_up(mutex);
		sem_up(empty_count);
		sleep(rand()%10);
	}
}

void DeleteSemaphoreSet(int id)
{
	if (semctl(id, 0, IPC_RMID, NULL) == -1)
		{
		perror("Error releasing semaphore!");
		exit(EXIT_FAILURE);
		}
}

void cleanup(){
	DeleteSemaphoreSet(mutex);
	DeleteSemaphoreSet(full_count);
	DeleteSemaphoreSet(empty_count);
}
int main(){
	printf("Hello worldd\n");
	// p = (struct buffer*)malloc(sizeof(struct buffer));
	// init(p);

	//semaphores
	if((mutex = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) == -1) {
		printf("%s\n", "Error creating sempahore");
	}
	if((semctl(mutex, 0, SETVAL, 1)) == -1){
		printf("%s\n", "Error setting value to semaphore");
	}

	if((full_count = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) == -1) {
		printf("%s\n", "Error creating sempahore");
	}
	if((semctl(full_count, 0, SETVAL, 0)) == -1){
		printf("%s\n", "Error setting value to semaphore");
	}
	if((empty_count = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) == -1) {
		printf("%s\n", "Error creating sempahore");
	}
	if((semctl(empty_count, 0, SETVAL, BUF_SIZE)) == -1){
		printf("%s\n", "Error setting value to semaphore");
	}
	//semaphores end





	//shared memory
	int shmid = AllocateSharedMemory(sizeof(struct buffer));
	p = (struct buffer *)MapSharedMemory(shmid);
	init(p);

	// pthread_mutex_init(&mutex, NULL);

	// sem_init(&empty_count, 0, BUF_SIZE);
	// sem_init(&full_count, 0, 0);

	int pid = fork();
	if(pid == 0){
		printf("%s\n", "in child");
		producer();
		// printf("%d\n", p->size);
	}
	else if(pid >0){
		printf("%s\n", "in parent");
		// printf("%d\n", p->size);
		consumer();
		int status;
		waitpid(-1, &status, 0 );
		cleanup();
		printf("DONE\n");
	}
	else{
		return 0;
	}
}
