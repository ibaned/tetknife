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

int main()
{
  static int const servport = 4243;
  int listenfd;
  int serverfd;
  int clientfd;
  void* buf;
  listenfd = server_posix_listen(servport);
  print("listening on port %d ...\n", servport);
  serverfd = server_posix_accept(listenfd);
  print("got a connection\n");
  make_nonblocking(serverfd);
  clientfd = client_posix_connect("127.0.0.1", 4242);
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
