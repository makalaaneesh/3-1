#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	char* dirpath; // creating a char* for 
	// if arg is not given, assume current working directory
	if(argc == 1){
		dirpath = (char*)malloc(sizeof(char));
		strcpy(dirpath,".");
	}

	else if(argc ==2){
		dirpath = (char*)malloc(sizeof(char)*strlen(argv[1]));
		strcpy(dirpath,argv[1]);
	}
	//opening the dir
	DIR* d = opendir(dirpath);
	if(d == NULL){
		printf("%s\n", "Direcotry does not exist\n");
		return 0;
	}
	//accessing the directory stream
	struct dirent *sd;
	while((sd = readdir(d))!= NULL){
		if(sd->d_name[strlen(sd->d_name)-1] == '~')
			continue;
		if(sd->d_name[0] == '.')
			continue;
		printf("%s\n", sd->d_name);
	}
	
	free(dirpath);
	closedir(d);
}