#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int rem(char *filepath, int prompt){
	if(prompt){
		char y_n; 

		printf("%s\n", "Do you want to delete?(y/n)");
		scanf("%c",&y_n);
		if(y_n == 'n')
			return 0;
	}
	unlink(filepath);
}

int main(int argc, char* argv[]){
	char *filepaths[argc-1];
	int i = argc -1;
	int c = 0;
	int prompt = 0;
	if (argv[1][0] == '-' && argv[1][1] == 'i'){
		prompt = 1;
	}
	while(argv[i][0] != '-' &&  !strcmp(argv[i],"./a.out")){
		// filepaths[c] = (char*)malloc(sizeof(char)*strlen(argv[i]));
		// strcpy(filepaths[c], argv[i]);
		// i--;
		// c++;
		rem(argv[i],prompt);
		i--;
	}
	
	
	

}