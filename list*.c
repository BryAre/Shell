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
  int pid1, pid2, status;
  errno = 0;

  //Open or create tree.txt file
  destfd = open("tree.txt", O_WRONLY | O_CREAT, S_IRWXU);
  if(-1 == destfd){
    printf("\n open() tree.txt file failed with error [%s]\n",strerror(errno));
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
    else{
      waitpid(pid2, &status, 0); //wait for child process to finish
      close(destfd); //close file
    }
    waitpid(pid1, &status, 0); //wait for child process to finish
  }
  return 0;
}
