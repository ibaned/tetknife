#ifndef FLAG_H
#define FLAG_H

#include "cad.h"

typedef struct {
  unsigned char* bytes;
} flag;

void flag_init(flag* f);
void flag_grow(flag* f, unsigned c);
void flag_clear(flag* f, int i);
void flag_set(flag* f, int i);
int flag_get(flag* f, int i);
void flag_dtor(flag* f);

typedef struct gflag gflag;

gflag* gflag_new(void);
void gflag_grow(gflag* f, gent_type t, unsigned c);
void gflag_clear(gflag* f, gent e);
void gflag_set(gflag* f, gent e);
int gflag_get(gflag* f, gent e);
void gflag_free(gflag* f);

#endif
