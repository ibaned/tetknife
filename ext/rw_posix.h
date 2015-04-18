static int try_write(int fd, void const* data, unsigned size)
{
  char const* p;
  ssize_t step;
  p = data;
  while (size) {
    errno = 0;
    step = write(fd, p, size);
    if (step == -1) {
      if (errno == EAGAIN)
        continue;
      else
        return 0;
    } else {
      size -= step;
      p += step;
    }
  }
  return 1;
}

static int try_read(int fd, void* data, unsigned size)
{
  char* p;
  ssize_t step;
  p = data;
  while (size) {
    errno = 0;
    step = read(fd, p, size);
    if (step == -1) {
      if (errno == EAGAIN)
        continue;
      else
        return 0;
    } else {
      size -= step;
      p += step;
    }
  }
  return 1;
}
