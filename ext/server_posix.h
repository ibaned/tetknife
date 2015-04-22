#include "rw_posix.h"

int server_posix_listen(int port)
{
  int listenfd;
  int err;
  struct sockaddr_in servaddr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  ASSERT(listenfd != -1);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);
  err = bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  if (err) {
    die("server_new couldn't bind to port %d\n"
        "wait around a bit or check if its in use\n", port);
  }
  err = listen(listenfd, 1);
  ASSERT(err != -1);
  return listenfd;
}

int server_posix_accept(int listenfd)
{
  int fd;
  fd = accept(listenfd, NULL, NULL);
  ASSERT(fd != -1);
  close(listenfd);
  return fd;
}
