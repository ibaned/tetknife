#include "../basics.h"
#define SILENCE_TRY_READ_UNUSED_WARNING
#include "client_posix.h"
#include "server_posix.h"
#include <fcntl.h>

static void make_nonblocking(int fd)
{
  int flags;
  int err;
  flags = fcntl(fd, F_GETFL, 0);
  ASSERT(flags >= 0);
  flags |= O_NONBLOCK;
  err = fcntl(fd, F_SETFL, flags);
  ASSERT(!err);
}

#define BUF_SIZE 4096

static int try_relaying(int fromfd, int tofd, void* buf)
{
  void* bufp;
  unsigned size_rem;
  size_rem = BUF_SIZE;
  bufp = buf;
  if (!try_read_some(fromfd, &bufp, &size_rem))
    return 0;
  if (size_rem != BUF_SIZE)
    if (!try_write(tofd, buf, BUF_SIZE - size_rem))
      return 0;
  return 1;
}

int main(int argc, char** argv)
{
  int servport;
  int listenfd;
  int serverfd;
  int clientfd;
  void* buf;
  if (argc != 4)
    die("usage: %s <listen port> <connect hostname> <connect port>\n", argv[0]);
  servport = my_atoi(argv[1]);
  listenfd = server_posix_listen(servport);
  serverfd = server_posix_accept(listenfd);
  print("got a connection\n");
  make_nonblocking(serverfd);
  clientfd = client_posix_connect(argv[2], my_atoi(argv[3]));
  make_nonblocking(clientfd);
  buf = my_malloc(BUF_SIZE);
  while (1) {
    if (!try_relaying(serverfd, clientfd, buf))
      break;
    if (!try_relaying(clientfd, serverfd, buf))
      break;
  }
  close(serverfd);
  close(clientfd);
  return 0;
}
