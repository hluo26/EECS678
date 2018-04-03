#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BSIZE 256
#define NSTRS 3
#define SOCKET_ADDRESS "mysock"

/*
 * This is the set of strings we will send through the socket for
 * conversion
 */
char *strs[NSTRS] = {
  "this is the first string from the client\n",
  "this is the second string from the client\n",
  "this is the third string from the client\n"
};

int main(int argc, char *argv[])
{
  int sockfd, ret, i;
  struct sockaddr_un saun;
  char buf[BSIZE];


  /* Add Code: Populate the sockaddr_un struct */
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, SOCKET_ADDRESS);

  /* Add Code: Create the client session socket */
    sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error Opening Socket");
    return EXIT_FAILURE;
  }

  /* Add Code: Connect the session socket to the server */
    
    ret = connect(sockfd,(struct sockaddr *) &saun,sizeof(saun));
  if (ret < 0) {
    perror("Error Connecting Sockets");
    return EXIT_FAILURE;
  }

  /* Add Code: Send the strs array, one string at a time, to the
   * server. Read the converted string and print it out before sending
   * the next string
   */
  for (i = 0; i < NSTRS; i++) {
    
    printf("SENDING:\n%s", strs[i]);
    write(sockfd, strs[i], BSIZE);
    read(sockfd, buf, BSIZE);
    printf("RECEIVED:\n%s\n", buf);
  }

  close(sockfd);

}
