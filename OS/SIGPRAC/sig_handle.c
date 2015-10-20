#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int sig_usr1_flag = 0;

void sig_action(int signo){
	if(signo == SIGUSR1){
		printf("Received sigusr1\n");
		sig_usr1_flag = 1;
	}
	else if (signo == SIGUSR2)
		printf("Received sigusr2\n");
	else
		printf("Received signal no %d\n",signo);

}


int main(){
	if(signal(SIGUSR1, sig_action) == SIG_ERR){
		printf("%s\n", "Error in catching SIGUSR1");
	}
	if(signal(SIGUSR2, sig_action) == SIG_ERR){
		printf("%s\n", "Error in catching SIGUSR2");
	}
	// for(;;)
	// 	pause();
	while(sig_usr1_flag == 0){
		pause();
		printf("waiting\n");
	}
	printf("This is after having waited for the signal to occur.\n");
}