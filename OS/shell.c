
#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int printerror(){
	printf("%s\n", "there is some error in the format. Please run help for the given command");
}


int trim(char *token){
	if (token[strlen(token)-1] == '\n')
		token[strlen(token)-1] = '\0';
}


int execute(char **argv, int argc, char *infile, char*outfile, int bg){
	argv[argc] = NULL;
	int pid;
	int status;
	int saved_out;
	int saved_in;
	int in;
	int out;
	
	if(infile!= NULL){
		saved_in = dup(0);
		in = open(infile, O_RDONLY);
		dup2(in, 0);
		close(in);
	}
	if(outfile){
		saved_out = dup(1);
		out = open(outfile, O_WRONLY | O_TRUNC);
		if(out == -1){
			mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
			creat(outfile,mode);
			out = open(outfile,O_WRONLY | O_TRUNC );
		}
		dup2(out,1);
		close(out);
		
	}

	if((pid = fork()) < 0){
		printerror();
	}				
	else if (pid == 0){//child
		
		if(execvp(*argv,argv) < 0){
			printf("exec\n");
			printerror();
		}
		exit(1);
	}
	else{
		waitpid(-1, &status, 0);
		if (infile){
			close(0);
			dup2(saved_in, 0);
		}
		if (outfile){
			close(1);
			dup2(saved_out,1);
		}
	}	
}

int main(){

	char buf[50];
	char *cmd = NULL;
	char *argv[10];
	int argc = 0;
	char *token;
	char *outfile = NULL;
	char *infile = NULL;
	while(1){
		argc = 0;
		outfile = NULL;
		infile = NULL;
		printf("\n$myshell>" );
		if (fgets (buf, 100, stdin) == NULL)
			break;
		token = strtok(buf," ");
		trim(token);

		if (strcmp(buf,"exit") == 0)
			break;	
		cmd = (char *) malloc(sizeof(char) * 20);
		strcpy(cmd,token);
		argv[argc] = (char*) malloc(sizeof(char)* 20);
		strcpy(argv[argc],token);
		argc++;

		while(token = strtok(NULL," ")){
			trim(token);
			if (strcmp(token,">") == 0){
				token = strtok(NULL," ");
				trim(token);
				if(token == NULL){
					printerror();
					exit(0);
				}
				outfile = (char*)malloc(sizeof(char)* 20);
				strcpy(outfile,token);

			}
			else if (strcmp(token,"<") == 0){
				token = strtok(NULL," ");
				trim(token);
				if(token == NULL){
					printerror();
					exit(0);
				}
				infile = (char*)malloc(sizeof(char)* 20);
				strcpy(infile,token);
				// printf("infile is %s\n", infile);
			}
			else{
				argv[argc] = (char*) malloc(sizeof(char)* 20);
				strcpy(argv[argc],token);
				argc++;
			}
		}
		// printf("command: %s\n", cmd);
		// printf("arguments\n");
		// int i ;
		// for (i = 0; i < argc; ++i)
		// {
		// 	printf("%s\t", argv[i]);
		// }
		// if(infile){
		// 	printf("infile: %s\n", infile);
		// }
		// if(outfile){
		// 	printf("outfile %s\n", outfile);
		// }
		execute(argv,argc,infile, outfile,0);
																																																	

	}
}