#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void shell_s();

void tree_s();

void exit_s();

void path_s();

void list_s();

int main(int argc, char *argv[]){
	shell_s();
}

void shell_s(){
	int child, status;
	char input[100];

	printf(":");
	fgets(input, 100, stdin);

	strtok(input, "\n");


	while(strcmp(input, "exit") != 0){
		child = fork();
		if(child == 0){
			if(strcmp(input, "tree") == 0)
				tree_s();
			else if(strcmp(input, "list") == 0)
				list_s();
			else if(strcmp(input, "path") == 0)
				path_s();
			else{
				printf("Command not found\n");
				exit(0);
			}
				
		}
		
		else{
			waitpid(child, &status, 0);
			printf(":");
			fgets(input, 100, stdin);
			strtok(input, "\n");
		}
	}
	
	exit_s();

}
