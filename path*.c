#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {

	int t2Write; //opens tf.txt to write pwd

	// FILE NAMES
	char oldt2File[] = "./Dir0/t2.txt";
	char newt2File[] = "./Dir0/path-info.txt";

	char oldt3File[] = "./Dir0/t3.txt";
	char newt3File[] = "./Dir0/log.txt";

	char treeFile[] = "./Dir0/tree.txt";


	char ch; //characters read in file

	FILE *pathinfoRead,*tree,*t3; //for concatentation

	int pid1,pid2,status;//For children when calling execlp

	pid1 = fork();
	if (pid1 == 0) { //First child forked for exec(pwd)
		execlp("pwd","pwd",NULL);
	}
	else{
		pid2 = fork();
		if (pid2 == 0){ //Second child forked for writing pwd to t2.txt
			t2Write = open(oldt2File, O_WRONLY);
			dup2(t2Write,1); //write towards t2.txt
			execlp("pwd","pwd",NULL);
		}
		else{
			waitpid(pid2,&status,0);//Parent waits for second child to finish
			close(t2Write);
		}
		waitpid(pid1,&status,0); //Parent waits for first child to finish
		rename(oldt2File,newt2File);//Rename t2.txt to path-info.txt

		//Concatentation, open all files
		pathinfoRead = fopen("./Dir0/path-info.txt","r");
		tree = fopen("./Dir0/tree.txt","r");
		t3 = fopen("./Dir0/t3.txt","w");

		while((ch = fgetc(pathinfoRead)) != EOF){//write all contents of path-info to t3
			fputc(ch,t3);
		}
		while((ch = fgetc(tree)) != EOF){
			fputc(ch,t3);
		}

		fclose(pathinfoRead);
		fclose(tree);
		fclose(t3);

		rename(oldt3File,newt3File); //rename t3.txt to log.txt
		remove(treeFile);
		remove(newt2File);
	}


	return 0;
}
