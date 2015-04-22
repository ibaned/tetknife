#include "../server.h"
#include "../basics.h"

#include "server_posix.h"

server* server_new(int port)
{
  server* s;
  s = my_malloc(sizeof(*s));
  s->listenfd = server_posix_listen(port);
  s->fd = -1;
  return s;
}

void server_accept(server* s)
{
  s->fd = server_posix_accept(s->listenfd);
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

