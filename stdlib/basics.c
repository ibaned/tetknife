#include "../basics.h"
#include <stdarg.h>
#include <stdio.h>

void debug(char const* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}
