/*CSC 33200 Operating Systems - Group Project
 *Group A
 *Members: Angel Baez, Arun Ajay, Bryan Arevalo, Alexandria Guo, Linda Wong
 *Assigned Parts:Shell - Angel
 *Tree - Linda
 *List - Alexandria
 *Path - Arun
 *Exit - Bryan
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void shell_s();

void tree_s();

void exit_s(char *commands, int tracker, int count);

void path_s();

void list_s();

//The main function calls shell, which is responsible for the rest
//of the program flow
int main(int argc, char *argv[]){
	shell_s();
}

/*list_s: this new command will clear the terminal screen and print a detailed list of all content of
the current directory (similar to ls -l) to the terminal and t1.txt. Finally, it will change the name of text file to tree.txt.
*/
void list_s() {
	int destfd;
	int renameFile;
	char filename[] = "./Dir0/t1.txt";
	char newFilename[] = "./Dir0/tree.txt";
	int pid1, pid2, status;
	errno = 0;

	//Open or create tree.txt file
	destfd = open(filename, O_WRONLY | O_CREAT, S_IRWXU);
	if(-1 == destfd){
		printf("\n open() t1.txt file failed with error [%s]\n",strerror(errno));
		return;
	}

	pid1 = fork();
	if (pid1==0) { //child forked
		system("clear"); //clear terminal screen
		execlp("ls", "ls", "-l", NULL); //execute ls -l;
		printf ("EXECVP Failed\n"); //print only if failed
	}

	else{ //parent
		waitpid(pid1, &status, 0); //wait for child process to finish
		pid2 = fork();
		if (pid2==0) { //child forked
			dup2(destfd, 1); //make stdout print to file
			execlp("ls", "ls", "-l", NULL); //execute ls -l;
			printf ("EXECVP Failed\n"); //print only if failed
		}

		else{ //parent
			waitpid(pid2, &status, 0); //wait for child process to finish
			close(destfd); //close file
		}

		waitpid(pid1, &status, 0); //wait for child process to finish
		//rename t1.txt to tree.txt
		renameFile = rename(filename, newFilename);

		if (renameFile != 0) {
			printf("Renaming t1.txt to tree.txt failed\n");
		}
	}
	return;
}

/* tree_s : this new command will create a directory and call it Dir0. Then it will
 * change the working directory to Dir0, and create three empty text files namely,
 * t1.txt, t2.txt, and t3.txt, and one empty directory called Dir1, inside it
 */
void tree_s(){
	int pid1, status;
	int ret1, ret2, file1, file2, file3;
	struct stat st = {0};
	pid1 = fork();

	if (pid1==0) {
		// check if directory Dir0 exists already
		if(stat("./Dir0", &st) == -1){
			// make Dir0
			ret1 = mkdir("./Dir0", S_IRWXU);
		}

		// change working directory to Dir0
		if(chdir("./Dir0") != 0){
			perror("Failed to change directory Dir0!");
		}

		else {
		// create 3 empty text files
			file1 = open("t1.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
			if(-1 == file1){
			printf("\nCannot create txt1.txt");
			return;
		}

		file2 = open("t2.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
		if(-1 == file2){
			printf("\nCannot create txt2.txt");
			return;
		}

		file3 = open("t3.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
		if(-1 == file3){
			printf("\nCannot create txt3.txt");
			return;
		}

		// check if directory Dir1 exists already
		if(stat("./Dir1", &st) == -1){
			// create Dir1
			ret2 = mkdir("./Dir1", S_IRWXU);
		}

		execlp("ls", "ls", "-F", NULL);
		// confirmation message
		// printf("%s\n", "Change to directory Dir0 and textfiles created successful!");
		}
	}
	else{
		waitpid(pid1, &status, 0);
		// close files
		close(file1);
		close(file2);
		close(file3);
	}
	return;
}

void shell_s(){
    //input holds the user input and commands
    //track the last 4 commands entered
	char input[100], commands[4];
	int tracker = 0;
	int commandcount = 0;

	//The symbol we use is the colon for our shell
	printf(":");
	fgets(input, 100, stdin);

	strtok(input, "\n");

	//Loop while the command entered is not exit
	while(strcmp(input, "exit") != 0){
		//Depending on user input, different commands will be executed
		if(strcmp(input, "tree") == 0){
			commands[tracker] = 't';
            tracker = (tracker + 1) % 4;
       	    commandcount++;
			tree_s();
		}

		else if(strcmp(input, "list") == 0){
			commands[tracker] = 'l';
			tracker = (tracker + 1) % 4;
        	commandcount++;
			list_s();
		}

		else if(strcmp(input, "path") == 0){
			commands[tracker] = 'p';
			tracker = (tracker + 1) % 4;
       		commandcount++;
			path_s();
		}

		//If no valid command found, message displayed
		else{
			printf("Command not found\n");
		}
	

		printf(":");
		fgets(input, 100, stdin);
		strtok(input, "\n");
	}
	
	//Exit command executed when exit entered
	exit_s(commands, tracker, commandcount);

}

void path_s() {

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
	return;
}

void exit_s(char *commands, int tracker, int count){
	char ch;

	if(count > 4)
		count = 4;
	if(count > 0){
		printf("Last %d commands exectued:\n", count);
		for(int i = 0; i <= count; i++){
		if(tracker == -1)
			tracker = 3;

			else if(commands[tracker] == 'l')
				printf("list\n");
			else if(commands[tracker] == 'p')
				printf("path\n");
			else if(commands[tracker] == 't')
				printf("tree\n");

			tracker--;
		}
	}


	pid_t list_files;
	char *argv[10];

	list_files = fork(); /* fork child A */
	if (list_files == 0) /* execute child code */
	{
		argv[1] = "ls";
		argv[2] = "-l";
		execlp(argv[1], argv[1], argv[2], (char *)NULL);
	}

	else if (list_files < 0) /* problem forking child */
	{
		perror("An error occured: Could not fork child.");
	}

	wait(NULL);


	//infinite loop
	while (1)
	{
		printf("Press Return to Exit: ");
		//read a single character
		ch = fgetc(stdin);

		if (ch == 0x0A)
		{
			printf("Success.\n");
			return;
		}

		//read dummy character to clear
		//input buffer, which inserts after character input
		ch = getchar();
	}
}
