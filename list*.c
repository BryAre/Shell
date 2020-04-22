/*CSC 33200 Operating Systems - Group Project
Alexandria Guo
list*
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

int main(int argc, char const *argv[]) {
  int destfd;
  int renameFile;
  char filename[] = "t1.txt";
  char newFilename[] = "tree.txt";
  int pid1, pid2, pid3, status;
  errno = 0;

  //Open or create tree.txt file
  destfd = open(filename, O_WRONLY | O_CREAT, S_IRWXU);
  if(-1 == destfd){
    printf("\n open() t1.txt file failed with error [%s]\n",strerror(errno));
    return 1;
  }

  pid1 = fork();
  if (pid1==0) { //child forked
    system("clear"); //clear terminal screen
    execlp("ls", "ls", "-l", NULL); //execute ls -l;
    printf ("EXECVP Failed\n"); //print only if failed
  }
  else{ //parent
    pid2 = fork();
    if (pid2==0) { //child forked
      dup2(destfd, 1); //make stdout print to file
      execlp("ls", "ls", "-l", NULL); //execute ls -l;
      printf ("EXECVP Failed\n"); //print only if failed
    }
    else{ //parent
      waitpid(pid2, &status, 0); //wait for child process to finish
      close(destfd); //close file
      pid3 = fork();
      if (pid3==0) { //child forked
        renameFile = rename(filename, newFilename);
        if (renameFile != 0) {
          printf("Renaming t1.txt to tree.txt failed\n");
        }
      }
      else{ //parent
        waitpid(pid3, &status, 0);//wait for child process to finish
      }
    }
    waitpid(pid1, &status, 0); //wait for child process to finish
  }
  return 0;
}
