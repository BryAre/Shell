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
  pid_t pid1, pid2;
  int status1, status2;

  int destfd;
  int count = 1;
  char buff[100];
  errno = 0;

  //Create new file
  destfd = creat("tree.txt", S_IRWXU);

  //Opens files
  if(-1 == destfd){
    printf("\n create files failed with error [%s]\n",strerror(errno));
    return 1;
  }
  else{
    close(destfd);
  }

  //Create pipe
  int fd1[2];
  if (pipe(fd1) != 0) {
    fprintf(stderr, "Pipe Failed\n");
  }

  //Fork Child 1
  //output should be writen to pipe (cannot read)
  pid1 = fork();
  if (pid1 == 0) { //Fork() success
    system("clear");
    close(fd1[0]); //Close input/reading side of pipe
    dup2(fd1[1],1); //Close stdout; copies and points write-end of pipe to stdout(1)
    execlp("ls", "ls", "-l", NULL); //execute ls -F; -1 for vertical listing

  }
  else if (pid1 < 0) {
    fprintf(stderr, "Child 1 Fork Failed\n");
    return 1;
  }
  //Parent Process
  else{
    //Fork Child 2
    //input should be read from pipe (cannot write)
    pid2 = fork();
    if (pid2 == 0) { //Fork() success
      close(fd1[1]); //Close output/writing side of pipe
      dup2(fd1[0], destfd);
      
    }
    else if (pid2 < 0) {
      fprintf(stderr, "Child 2 Fork Failed\n");
      return 1;
    }

    close(fd1[0]); //Close input/reading side of pipe
    close(fd1[1]); //Close output/writing side of pipe
    waitpid(pid1, &status1, 0);
    // printf("Child 1 Process ended\n");
    waitpid(pid2, &status2, 0);
    // printf("Child 2 Process ended\n");
    // printf("Parent Process ended\n");
  }
  return 0;
}
