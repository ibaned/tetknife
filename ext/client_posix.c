#include "../client.h"
#include "../basics.h"

#include "client_posix.h"

struct client {
  int fd;
};

client* client_new(char const* servname, int port)
{
  client* c;
  c = my_malloc(sizeof(*c));
  c->fd = client_posix_connect(servname, port);
  return c;
}

void client_free(client* c)
{
  close(c->fd);
  my_free(c);
}

int client_try_send(client* c, void const* data, unsigned size)
{
  return try_write(c->fd, data, size);
}

int client_try_recv(client* c, void* data, unsigned size)
{
  return try_read(c->fd, data, size);
}

void client_sleep(unsigned msecs)
{
  client_posix_sleep(msecs);
}
