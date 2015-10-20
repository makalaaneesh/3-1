#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]){
	int i = 1;
	int code;
	if (argv[1][0] == '-'){
		code = atoi(argv[1]+1);
	}
	else 
		code = 9;
	for(i = 1; i< argc;i++){
		if (argv[i][0] == '-')
			continue;
		int pid = atoi(argv[i]);
		kill(pid,code);
	}
}