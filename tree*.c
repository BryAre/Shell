/* File: tree*.c
 * CSC33200 : Operating Systems
 * Linda Wong
 * tree* : this new command will create a directory and call it Dir0. Then it will
 * change the working directory to Dir0, and create three empty text files namely,
 * t1.txt, t2.txt, and t3.txt, and one empty directory called Dir1, inside it
 */
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
  int ret1, ret2, file1, file2, file3;
  struct stat st = {0};
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
      return 1;
    }
    file2 = open("t2.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    if(-1 == file2){
      printf("\nCannot create txt2.txt");
      return 1;
    }
    file3 = open("t3.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    if(-1 == file3){
      printf("\nCannot create txt3.txt");
      return 1;
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
  // close files
  close(file1);
  close(file2);
  close(file3);

  return 0;
}
