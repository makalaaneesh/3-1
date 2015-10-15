#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

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
	// printf("Downing %d\n", semid);
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = 0;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s %d\n", "Error locking semaphore",errno);
	}
	// if(errno == ENOMEM){
	// 	printf("%s\n", "not enough memory");
	// }
	// else if(errno == E2BIG){
	// 	printf("%s\n", "number greater");
	// }
	// else if(errno == ERANGE){
	// 	printf("%s\n", "range");
	// }

}
void sem_up(int semid){
	// printf("Uping %d\n", semid);
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = 0;
	if((semop(semid, &sb,1)) == -1){ //id, struct, number of semaphores
		printf("%s %d\n", "Error unlocking semaphore", errno);
	}
}


/*--------------------------------------------------------------------------------------*/



int *resource; //the shared variable between the reader and the writer

int reader_count = 0;

int  readers_mutex, writers_mutex, db ;


void writer(int id){
	int i = id;
		printf("in writer %d\n", id);		




		// sem_down(writers_mutex);
		// (*resource) = (*resource) +1;
		// printf("Wrote value by writer#%d at %d\n",i, (*resource));
		// sem_up(writers_mutex);
		// // sleep(rand()%10);


}


void reader(int id){
	int i = id;
		printf("in reader %d\n", id);
		


		// //entry section 
		// sem_down(readers_mutex);
		// // sem_down(db); 
		// 	reader_count++;
		// 	if(reader_count == 1)
		// 		sem_down(writers_mutex);
		// // sem_up(db);
		// sem_up(readers_mutex);
		// //entry section

		// printf("Read value by reader #%d at %d\n",i, (*resource));

		// //exit section 
		// sem_down(readers_mutex);
		// // sem_down(db);
		// reader_count--;
		// if(reader_count == 0)
		// 	sem_up(writers_mutex);

		// // sem_up(db);
		// sem_up(readers_mutex);
		// //exit section
		// // sleep(rand()%5);
	
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
	printf("%d\n",EINVAL);// (set doesn
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

	// static int rc = 0;
	// int i;
	// for (i = 0; i < 10; ++i) {
	//     int pid = fork();
	//     if (pid) {
	//         continue;
	//     } else if (pid == 0) {
	    	
	// 	    	reader(i+1);
	// 	    	rc++;
	    	
	    	
	//         break;
	//     } else {
	//         printf("fork error\n");
	//         exit(1);
	//     }
	// }


	int pid = fork();
	if(pid == 0){//child
		int i;
		for(i = 0; i < 5; i++){
			int pid2 = fork();
			if (pid2 == 0){ //child
				reader(i+1);
				break;
			}
			else if (pid2){ //parent
				continue;
			}
			else{
				printf("fork error\n");
			}
		}
	}
	else{
		int pid3 = fork();
		if(pid3 == 0){ //child
		int i;
			for(i = 0; i<5;i++){
				int pid4 = fork();
				if(pid4 == 0){
					writer(i+1);
					break;
				}
				else if (pid4){
					continue;
				}
				else{
					printf("fork error\n");
				}
			}
		}
		else{
			int status;
		waitpid(-1,&status, 0);
		printf("DONe\n");
		}



		int status;
		waitpid(-1,&status, 0);
		printf("DONe\n");
	}


     // int status;
     // waitpid(-1,&status, 0);
     // cleanup();
	// printf("DONE\n");
}
