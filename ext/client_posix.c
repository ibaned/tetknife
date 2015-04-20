#include "../client.h"
#include "../basics.h"

/* seriously, guys. I just want to sleep for less
   than one second. figure it out. this is already
   just a hack to workaround sometime broken... */
#define _POSIX_C_SOURCE 200809L
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "rw_posix.h"

struct client {
  int fd;
};

client* client_new(char const* servname, int port)
{
  client* c;
  int err;
  struct hostent* serv;
  struct sockaddr_in servaddr;
  c = my_malloc(sizeof(*c));
  c->fd = socket(AF_INET, SOCK_STREAM, 0);
  ASSERT(c->fd != -1);
  serv = gethostbyname(servname);
  ASSERT(serv != NULL);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  memcpy(&servaddr.sin_addr.s_addr, serv->h_addr_list[0], serv->h_length);
  servaddr.sin_port = htons(port);
  err = connect(c->fd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  ASSERT(err >= 0);
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
  struct timespec ts;
  ts.tv_sec = msecs / 1000;
  ts.tv_nsec = (msecs % 1000) * 1000;
  nanosleep(&ts, NULL);
}
