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

void flag_dtor(flag* f)
{
  my_free(f->bytes);
}

struct gflag {
  flag f[CAD_ENT_TYPES];
};

gflag* gflag_new(void)
{
  gflag* f;
  gent_type t;
  f = my_malloc(sizeof(*f));
  for (t = 0; t < CAD_ENT_TYPES; ++t)
    flag_init(f->f + t);
  return f;
}

void gflag_grow(gflag* f, gent_type t, unsigned c)
{
  flag_grow(f->f + t, c);
}

void gflag_clear(gflag* f, gent e)
{
  flag_clear(f->f + e.t, e.i);
}

void gflag_set(gflag* f, gent e)
{
  flag_set(f->f + e.t, e.i);
}

int gflag_get(gflag* f, gent e)
{
  return flag_get(f->f + e.t, e.i);
}

void gflag_free(gflag* f)
{
  gent_type t;
  for (t = 0; t < CAD_ENT_TYPES; ++t)
    flag_dtor(f->f + t);
  my_free(f);
}
