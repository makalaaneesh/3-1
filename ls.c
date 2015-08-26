#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>


// int main(int argc, char* argv[]){
// 	char* dirpath; // creating a char* for 
// 	// if arg is not given, assume current working directory
// 	if(argc == 1){
// 		dirpath = (char*)malloc(sizeof(char));
// 		strcpy(dirpath,".");
// 	}

// 	else if(argc ==2){
// 		dirpath = (char*)malloc(sizeof(char)*strlen(argv[1]));
// 		strcpy(dirpath,argv[1]);
// 	}
// 	//opening the dir
// 	DIR* d = opendir(dirpath);
// 	if(d == NULL){

// 		printf("%s\n", "Direcotry does not exist\n");
// 		return 0;
// 	}
// 	//accessing the directory stream
// 	struct dirent *sd;
// 	while((sd = readdir(d))!= NULL){
// 		if(sd->d_name[strlen(sd->d_name)-1] == '~')
// 			continue;
// 		if(sd->d_name[0] == '.')
// 			continue;
// 		printf("%s\n", sd->d_name);
// 	}
	
// 	free(dirpath);
// 	closedir(d);
// }


#define a_command 0
#define l_command 1
#define R_command 2

int print_file_details(struct stat *buf){
	
	printf("%ld\t", (long) buf->st_ino);
	printf( (buf->st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf->st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf->st_mode & S_IXUSR) ? "x" : "-");
    printf("\t");
    printf("%lld\t",(long long) buf->st_blocks);
    if (S_ISDIR(buf->st_mode)){
    	printf("%s\t", "dir");
    }
    else{
    	printf("%s\t", "fil");
    }

}


int print_details(struct dirent *sd, bool *options, char * dirpath){
	bool rec = false;
	if (options[l_command]){
		char *filename = (char*)malloc(sizeof(char) * strlen(sd->d_name));
		strcpy(filename, sd->d_name);
		struct stat * buf = (struct stat*)malloc(sizeof(struct stat));
		stat(filename, buf);
		print_file_details(buf);
		if (S_ISDIR(buf->st_mode) && options[R_command]){
			print_file_name(sd);
			chdir(sd->d_name);
			function(".", options);
			chdir("..");
			rec = true;
		}
	}
	if(!rec)
		print_file_name(sd);
}

int print_file_name(struct dirent *sd){
	printf("%s\n", sd->d_name);
}

int function(char *dirpath, bool* options){

	DIR *d = opendir(dirpath);
		if(d == NULL){
			printf("%s\n", "Direcotry does not exist\n");
			return 0;
		}
		struct dirent *sd;
		while((sd = readdir(d)) != NULL){
			if((sd->d_name[strlen(sd->d_name)-1] == '~') || (sd->d_name[0] == '.')){
				if (options[a_command]){
					print_details(sd,options,dirpath);
					
				}
				else{
					continue;
				}
				

			}
			print_details(sd,options,dirpath);
		}

	}


int main(int argc, char* argv[]){
	char *dirpath;
	if (argc == 1 || argv[argc-1][0] == '-'){
		dirpath = (char*)malloc(sizeof(char));
		strcpy(dirpath,".");
	}
	else{
		dirpath = (char*)malloc(sizeof(char)*strlen(argv[argc-1]));
		strcpy(dirpath,argv[argc-1]);
	}
	// printf("%s\n", dirpath);
	bool *options = (bool *)malloc(sizeof(bool)*3);
	options[0] = false;
	options[1] = false;
	options[2] = false;
	
	if(argc > 1){
		int i = 1;
		while((argv[i][0] == '-' )&& (i< argc)){
			int j;
			for(j = 1; j<strlen(argv[i]) ; j++){
				switch(argv[i][j]){
					case 'a': options[0] = true; break;
					case 'l': options[1] = true; break;
					case 'R': options[2] = true; break;
					default: break;
				}
			}
			i++;
			if(i>=argc)
				break;
		}
	}
	function(dirpath, options);
	// int i;
	// for (i = 0; i < 3; ++i)
	// {
	// 	printf("%d\n", options[i]);
	// }



}