#include "../server.h"
#include "../basics.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "rw_posix.h"

struct server {
  int listenfd;
  int fd;
};

server* server_new(int port)
{
  server* s;
  int err;
  struct sockaddr_in servaddr;
  s = my_malloc(sizeof(*s));
  s->listenfd = socket(AF_INET, SOCK_STREAM, 0);
  s->fd = -1;
  ASSERT(s->listenfd != -1);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);
  err = bind(s->listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  if (err)
    die("server_new couldn't bind to port %d\n", port);
  err = listen(s->listenfd, 1);
  ASSERT(err != -1);
  return s;
}

void server_accept(server* s)
{
  s->fd = accept(s->listenfd, NULL, NULL);
  ASSERT(s->fd != -1);
  close(s->listenfd);
  s->listenfd = -1;
}

void server_free(server* s)
{
  close(s->fd);
  my_free(s);
}

int server_try_send(server* s, void const* data, unsigned size)
{
  return try_write(s->fd, data, size);
}

int server_try_recv(server* s, void* data, unsigned size)
{
  return try_read(s->fd, data, size);
}

