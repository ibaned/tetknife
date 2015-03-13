#include "../basics.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void debug(char const* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}

void* my_malloc(unsigned long bytes)
{
  return malloc(bytes);
}

void* my_realloc(void* p, unsigned long bytes)
{
  return realloc(p, bytes);
}

void my_free(void* p)
{
  free(p);
}
