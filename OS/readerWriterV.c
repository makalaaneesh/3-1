#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>


int *resource; //the shared variable between the reader and the writer

int reader_count = 0;

int  readers_mutex, writers_mutex, db ;

/*-----------------------------Helper Functions -------------------------------------*/

int get_sem_val(int sid)
{
        return( semctl(sid, 0, GETVAL, 0));
}
int printsemvalues(){
	printf("readers mutex is %d, writers is %d\n", get_sem_val(readers_mutex), get_sem_val(writers_mutex));
}


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
	// printf("Downing %d\n", semid);
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s %d\n", "Error locking semaphore",errno);
	}

}
void sem_up(int semid){
	// printf("Uping %d\n", semid);
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s %d\n", "Error unlocking semaphore", errno);
	}
}


/*--------------------------------------------------------------------------------------*/





void writer(int id){
	int i = id;
		printf("in writer %d", id);	
		printsemvalues();	




		sem_down(writers_mutex);
		(*resource) = (*resource) +1;
		printf("Wrote value by writer#%d at %d\n",i, (*resource));
		sem_up(writers_mutex);
		// sleep(rand()%10);


}


void reader(int id){
	int i = id;
		printf("in reader %d", id);
		printsemvalues();


		//entry section 
		sem_down(readers_mutex);
		// sem_down(db); 
			reader_count++;
			if(reader_count == 1)
				sem_down(writers_mutex);
		// sem_up(db);
		sem_up(readers_mutex);
		//entry section

		printf("Read value by reader #%d at %d\n",i, (*resource));

		//exit section 
		sem_down(readers_mutex);
		// sem_down(db);
		reader_count--;
		if(reader_count == 0)
			sem_up(writers_mutex);

		// sem_up(db);
		sem_up(readers_mutex);
		//exit section
		// sleep(rand()%5);
	
}



void DeleteSemaphoreSet(int id)
{
	if (semctl(id, 0, IPC_RMID, NULL) == -1)
		{
		perror("Error releasing semaphore!");
		}
}

void cleanup(){
	DeleteSemaphoreSet(readers_mutex);
	DeleteSemaphoreSet(writers_mutex);
	// DeleteSemaphoreSet(db);
}

int main(){
	printf("Hello\n");
	// printf("%d\n", ERANGE);
	// printf("%d\n",ERANGE);
	// printf("%d\n",E2BIG); //(nsops greater than max number of ops allowed atomically)
	// printf("%d",EACCESS);// (permission denied)
	// printf("%d\n",EAGAIN); //(IPC_NOWAIT asserted, operation could not go through)
	// printf("%d\n",EFAULT); //(invalid address pointed to by sops argument)
	// printf("%d\n",EIDRM); //(semaphore set was removed)
	// printf("%d\n",EINTR );//(Signal received while sleeping)
	// printf("%d\n",EINVAL);// (set doesn
	// printf("%d\n",ENOMEM); //(SEM_UNDO asserted, not enough memory to create the
	      //undo structure necessary)
	// printf("%d\n", ERANGE);// 



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
	// if((db=semget(IPC_PRIVATE,1,0666|IPC_CREAT))==-1){
 //          printf("error in creation semaphore\n");
 //     }
 //     if((semctl(db,0,SETVAL,1))==-1){
 //            printf("error to set value\n");
 //     }
	//semaphores end





	//shared memory
	int shmid = AllocateSharedMemory(sizeof(int));
	resource = (int *)MapSharedMemory(shmid);
	*resource = 0;


	//loop for creating n identical processes.
	int i;
	int pid[5];
	for(i = 0; i<10 ; i++){
		pid[i]= fork();
		if(pid[i] < 0){
			printf("error in fork \n");
			exit(-1);
		}
		else if (pid[i] == 0){
			if(i%2 == 0)
				writer(i+1);
			else
				reader(i+1);
			exit(0);
		}
	}
	int c = 0;
	int status;

	int p;
	while (p = waitpid(-1, NULL, 0)) { //waiting for all child processes to terminate
	   if (errno == ECHILD) {
	      break;
	   }
	}
	printf("done!\n");
	cleanup();
}
