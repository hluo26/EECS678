#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

#define WRITE 1
#define READ 0

int main(int argc, char *argv[])
{
  int status;
  pid_t pid_1, pid_2, pid_3, pid_4;
  int a[2],b[2],c[2];
  char cmdbuf[BSIZE];

  if (argc != 4) {
    printf("usage: finder DIR STR NUM_FILES\n");
    exit(0);
  }
  pipe(a);
  pid_1 = fork();
  if (pid_1 == 0) {
    /* First Child */
    dup2(a[WRITE],STDOUT_FILENO);//pipe duplication
    close(a[WRITE]);
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s %s -name \'*\'.[ch]", FIND_EXEC, argv[1]);
    //close each pipes end
    if ( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0)) < 0) {
     fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
     return EXIT_FAILURE;
   }
    exit(0);
  }
  close(a[WRITE]);

  pipe(b);
  pid_2 = fork();
  if (pid_2 == 0) {
    /* Second Child */
    dup2(a[READ],STDIN_FILENO);//pipe duplication
    close(a[READ]);
    dup2(b[WRITE],STDOUT_FILENO);
    close(b[WRITE]);
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s %s -c %s", XARGS_EXEC, GREP_EXEC, argv[2]);
    //close each pipes end
    if ( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0)) < 0) {
     fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
     return EXIT_FAILURE;
   }

    exit(0);
  }
  close(a[READ]);
  close(b[WRITE]);

  pipe(c);
  pid_3 = fork();
  if (pid_3 == 0) {
    /* Third Child */
    dup2(b[READ],STDIN_FILENO);
    close(b[READ]);
    dup2(c[WRITE],STDOUT_FILENO);
    close(c[WRITE]);
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s -t : +1.0 -2.0 --numeric --reverse", SORT_EXEC);
    //close each pipes end
    if ( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0)) < 0) {
        fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
        return EXIT_FAILURE;
    }
    exit(0);
  }
  close(b[READ]);
  close(c[WRITE]);


  pid_4 = fork();
  if (pid_4 == 0) {
    /* Fourth Child */
    dup2(c[READ],STDIN_FILENO);
    close(c[READ]);
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "--lines=%s", argv[3]);
    //close each pipes end
    if ( (execl(HEAD_EXEC, BASH_EXEC, cmdbuf, (char *) 0)) < 0) {
      fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }
  close(c[READ]);

  if ((waitpid(pid_1, &status, 0)) == -1) {
    fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_2, &status, 0)) == -1) {
    fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_3, &status, 0)) == -1) {
    fprintf(stderr, "Process 3 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_4, &status, 0)) == -1) {
    fprintf(stderr, "Process 4 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }

  return 0;
}
