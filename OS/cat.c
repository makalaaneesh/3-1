#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int print(char *data, int count, char p){
	int n = 2;
	printf("%s", "1\t:");
	int i;
	switch(p){
		case 'n':
			for(i = 0; i<count;i++){
				printf("%c", data[i]);
				if(data[i] == '\n'){
					char *num = (char*)malloc(sizeof(char)*5);
					snprintf (num, sizeof(num), "%d\t:",n);
					printf("%s", num);
					n++;
				}
			}
	}
	printf("\n");
}

int main(int argc, char* argv[]){

	char *filepath;
	char *parameters;
	if(argc == 1){
		printf("%s\n", "Please specify a filename\n");
		return 0;
	}
	else if (argc == 2){
		
		if(argv[1][0] == '-'){
			printf("%s\n", "Please specify a filename\n");
			return 0;
		}
		else{
			filepath = (char*)malloc(sizeof(char)*strlen(argv[1]));
			strcpy(filepath, argv[1]);
		}
	}
	else if(argc == 3){
		filepath = (char*)malloc(sizeof(char)*strlen(argv[2]));
		strcpy(filepath, argv[2]);
		parameters = (char*)malloc(sizeof(char)*strlen(argv[1]));
		strcpy(parameters,argv[1]+1);

	}

	FILE *fp = fopen(filepath, "r");
	if(fp == NULL){
		printf("%s\n", "File does not exist\n");
		return 0;
	}
	int fd = fileno(fp);
	char data[1024];
	int count = read(fd,data,1024);
	data[count-1]='\0';
	int i;
	if(argc == 3){
		for(i=0;parameters[i]!='\0';i++){
			print(data, count, parameters[i]);
		}
	}
	else{
		printf("%s\n", data);
	}


}