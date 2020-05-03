#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void shell*();

int main(int argc, char *argv[]){
	shell*();
}

void shell*(){
	int child, status;
	char input[100];

	printf(":");
	fgets(input, 100, stdin);

	strtok(input, "\n");


	while(strcmp(input, "exit") != 0){
		child = fork();
		if(child == 0){
			if(strcmp(input, "tree") == 0)
				tree*();
			else if(strcmp(input, "list") == 0)
				list*();
			else if(strcmp(input, "path") == 0)
				path*();
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
	
	exit*();

}
