#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TESTCASES 10


int resource = 0;
int reader_count = 0;

pthread_mutex_t readers_mutex, writers_mutex;


void* writer(void *arg){
	int i = *((int*)arg);	
			
		pthread_mutex_lock(&writers_mutex);
		resource = resource +1;
		printf("Wrote value by writer#%d at %d\n",i, resource);
		pthread_mutex_unlock(&writers_mutex);
		// sleep(rand()%10);

	
	pthread_exit(0);

}


void* reader(void *arg){
	int i = *((int*)arg);
	
		//entry section 
		pthread_mutex_lock(&readers_mutex);
		reader_count++;
		if(reader_count == 1)
			pthread_mutex_lock(&writers_mutex);

		pthread_mutex_unlock(&readers_mutex);
		//entry section

		printf("Read value by reader #%d at %d\n",i, resource);

		//exit section 
		pthread_mutex_lock(&readers_mutex);
		reader_count--;
		if(reader_count == 0)
			pthread_mutex_unlock(&writers_mutex);

		pthread_mutex_unlock(&readers_mutex);
		//exit section
		// sleep(rand()%5);
	
	pthread_exit(0);
}

int main(){
	pthread_t r[10], w[10];
	pthread_mutex_init(&readers_mutex, NULL);
	pthread_mutex_init(&writers_mutex, NULL);
	int i =0;
	for (i = 0; i < TESTCASES; ++i)
	{
		int *arg = (int*)malloc(sizeof(int));
		*arg = i;
		pthread_create(&r[i], NULL, reader, arg);
		// sleep(rand()%5);
		pthread_create(&w[i], NULL, writer, arg);
		// sleep(rand()%1);
	}
	
	for (i = 0; i < TESTCASES; ++i)
	{
		pthread_join(r[i], NULL);
		pthread_join(w[i], NULL);
	}
	
	printf("DONE\n");
}
