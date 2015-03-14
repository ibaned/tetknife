#include "flag.h"
#include "basics.h"

void flag_init(flag* f)
{
  f->bytes = 0;
}

void flag_grow(flag* f, unsigned c)
{
  unsigned nbytes;
  nbytes = c / 8;
  if (c % 8)
    nbytes++;
  REALLOC(f->bytes, nbytes);
}

void flag_clear(flag* f, int i)
{
  unsigned byte = (unsigned)i / 8;
  unsigned bit = (unsigned)i % 8;
  f->bytes[byte] &= ~(1 << bit);
}

void flag_set(flag* f, int i)
{
  unsigned byte = (unsigned)i / 8;
  unsigned bit = (unsigned)i % 8;
  f->bytes[byte] |= (1 << bit);
}

int flag_get(flag* f, int i)
{
  unsigned byte = (unsigned)i / 8;
  unsigned bit = (unsigned)i % 8;
  return (f->bytes[byte] & (1 << bit)) != 0;
}

void flag_free(flag* f)
{
  my_free(f->bytes);
}

