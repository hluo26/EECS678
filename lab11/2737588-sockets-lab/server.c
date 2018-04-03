#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define QSIZE 5
#define BSIZE 256
#define SOCKET_ADDRESS "mysock"

/*
 * Convert a null-terminated sting (one whose end is denoted by a byte
 * containing '\0') to all upper case letters by starting at the
 * beginning and going until the null byte is found.
 */
void
convert_string (char *cp)
{
  char *currp;	/* pointer to current position in the input string */
  int c;        /* return value of toupper is the converted letter */

  for (currp = cp; *currp != '\0'; currp++) {
    c = toupper (*currp);
    *currp = (char) c;
  }
}

int main(int argc, char *argv[])
{
  int handshake_sockfd, session_sockfd, ret;
  struct sockaddr_un saun;
  char buf[BSIZE];


  /* Add Code: Populate the sockaddr_un struct */
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, SOCKET_ADDRESS);
  /* Add Code: Create the handshake socket */
    handshake_sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (handshake_sockfd < 0) {
    perror("Error Opening Socket");
    return EXIT_FAILURE;
  }


  /*
   * We need to unlink the address before binding to ensure the
   * address is free before attempting to bind.
   */
  unlink(SOCKET_ADDRESS);

  /* Add Code: Bind the handshake socket to the sockaddr. */
    ret = bind(handshake_sockfd,(struct sockaddr *) &saun,sizeof(saun));
  if (ret < 0) {
    perror("Error Binding Socket");
    return EXIT_FAILURE;
  }

  /* Add Code: Make the handshake socket a listening socket, with a
   * specified Queue Size
   */
    ret = listen(handshake_sockfd,sizeof(saun));
  if (ret < 0) {
    perror("Error Listening on Socket");
    return EXIT_FAILURE;
  }

  /* Add Code: Accept a connection on the handshake socket,
   * giving the session socket as the return value.
   */
    session_sockfd = accept(handshake_sockfd,NULL,NULL);
  if (session_sockfd < 0) {
    perror("Error Accepting Socket");
    return EXIT_FAILURE;
  }

  /* Add Code: Read lines one at a time from the connected session
   * socket. Convert each line to uppercase using convert_string, and
   * write the line back to the client. Continue until there are no
   * more lines to read.
   */
   while (read(session_sockfd, buf, BSIZE)>0) {
     printf("RECEIVED:\n%s", buf);
     convert_string(buf);
     write(session_sockfd, buf, BSIZE);
     printf("SENDING:\n%s\n", buf);
   }


  close(session_sockfd);
  close(handshake_sockfd);

}
