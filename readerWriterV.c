#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>

#define TESTCASES 10

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



int *resource; //the shared variable between the reader and the writer

int reader_count = 0;

int  readers_mutex, writers_mutex, db ;


void writer(int id){
	int i = id;
			
		sem_down(writers_mutex);
		*resource = *resource +1;
		printf("Wrote value by writer#%d at %d\n",i, *resource);
		sem_up(writers_mutex);
		// sleep(rand()%10);

	
	pthread_exit(0);

}


void reader(int id ){
	int i = id;
	
		//entry section 
		sem_down(readers_mutex);
		sem_down(db); 
			reader_count++;
			if(reader_count == 1)
				sem_down(writers_mutex);
		sem_up(db);
		sem_up(readers_mutex);
		//entry section

		printf("Read value by reader #%d at %d\n",i, *resource);

		//exit section 
		sem_down(readers_mutex);
		sem_down(db);
		reader_count--;
		if(reader_count == 0)
			sem_up(writers_mutex);

		sem_up(db);
		sem_up(readers_mutex);
		//exit section
		// sleep(rand()%5);
	
	pthread_exit(0);
}

int main(){
	//semaphores
	if((readers_mutex = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) == -1) {
		printf("%s\n", "Error creating sempahore");
	}
	if((semctl(readers_mutex, 0, SETVAL, 1)) == -1){
		printf("%s\n", "Error setting value to semaphore");
	}

	if((writers_mutex = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) == -1) {
		printf("%s\n", "Error creating sempahore");
	}
	if((semctl(writers_mutex, 0, SETVAL, 1)) == -1){
		printf("%s\n", "Error setting value to semaphore");
	}
	if((db=semget(IPC_PRIVATE,1,0666|IPC_CREAT))==-1){
          printf("error in creation semaphore\n");
     }
     if((semctl(db,0,SETVAL,1))==-1){
            printf("error to set value\n");
     }
	//semaphores end





	//shared memory
	int shmid = AllocateSharedMemory(sizeof(int));
	resource = (int *)MapSharedMemory(shmid);
	*resource = 0;
	int x, i;
	for(i=0;i<5;i++)
     {
          x=fork();
          if(x==0)
                writer(i+1);
          x=fork();
          if(x==0)
                reader(i+1);
     }
	printf("DONE\n");
}
