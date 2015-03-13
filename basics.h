#ifndef BASICS_H
#define BASICS_H

void debug(char const* format, ...)
  __attribute__((format(printf, 1, 2)));

void* my_malloc(unsigned long bytes);
void* my_realloc(void* p, unsigned long bytes);
void my_free(void* p);

#endif
