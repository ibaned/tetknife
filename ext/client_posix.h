/* seriously, guys. I just want to sleep for less
   than one second. figure it out. this is already
   just a hack to workaround sometime broken... */
#define _POSIX_C_SOURCE 200809L
#include <time.h>

#include "rw_posix.h"

static void client_posix_sleep(unsigned msecs)
{
  struct timespec ts;
  ts.tv_sec = msecs / 1000;
  ts.tv_nsec = (msecs % 1000) * 1000;
  nanosleep(&ts, NULL);
}

static int client_posix_connect(char const* servname, int port)
{
  int fd;
  int err;
  struct hostent* serv;
  struct sockaddr_in servaddr;
  char* errs;
  fd = socket(AF_INET, SOCK_STREAM, 0);
  ASSERT(fd != -1);
  while (1) {
    errno = 0;
    serv = gethostbyname(servname);
    if (serv)
      break;
    if (errno == EAGAIN) {
      debug("gethostbyname temporarily unavailable\n"
            "trying again in 1 second...\n");
      client_posix_sleep(1000);
      continue;
    }
    errs = strerror(errno);
    die("gethostbyname failed: %s\n", errs);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  memcpy(&servaddr.sin_addr.s_addr, serv->h_addr_list[0], serv->h_length);
  servaddr.sin_port = htons(port);
  errno = 0;
  err = connect(fd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  if (err) {
    errs = strerror(errno);
    die("connect failed: %s\n", errs);
  }
  return fd;
}
