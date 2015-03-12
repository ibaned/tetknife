#ifndef BASICS_H
#define BASICS_H

void debug(char const* format, ...)
  __attribute__((format(printf, 1, 2)));

#endif
