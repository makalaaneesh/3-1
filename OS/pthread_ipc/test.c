#include <pthread.h>
#include <stdio.h>



void printids(const char *s){
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid: %lu tid: %lu \n", s, (unsigned long)pid, (unsigned long)tid);
}


void* thread_function(void* arg){
	printids("new thread: ");
	return ((void*)0);
}

pthread_t ntid;

int main(){
	int err;
	err = pthread_create(&ntid, NULL, thread_function, NULL);
	if (err != 0)
		printf("%s\n", "can't create thread"); 
	printids("main thread: ");
	sleep(1);
	exit(0);
	
}