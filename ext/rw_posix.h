#ifndef RW_POSIX_H
#define RW_POSIX_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>

static int try_write_some(int fd, void const** data, unsigned* size)
{
  char const* p;
  ssize_t step;
  p = *data;
  errno = 0;
  step = write(fd, p, *size);
  if (step == -1) {
    if (errno == EAGAIN)
      return 1;
    else
      return 0;
  } else {
    *size -= step;
    p += step;
    *data = p;
  }
  return 1;
}

static int try_read_some(int fd, void** data, unsigned* size)
{
  char* p;
  ssize_t step;
  p = *data;
  errno = 0;
  step = read(fd, p, *size);
  if (step == -1) {
    if (errno == EAGAIN)
      return 1;
    else
      return 0;
  } else {
    *size -= step;
    p += step;
    *data = p;
  }
  return 1;
}

static int try_write(int fd, void const* data, unsigned size)
{
  while (size)
    if (!try_write_some(fd, &data, &size))
      return 0;
  return 1;
}

#ifndef SILENCE_TRY_READ_UNUSED_WARNING
static int try_read(int fd, void* data, unsigned size)
{
  while (size)
    if (!try_read_some(fd, &data, &size))
      return 0;
  return 1;
}
#endif

struct server {
  int listenfd;
  int fd;
};

#endif
