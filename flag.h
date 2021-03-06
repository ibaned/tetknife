#ifndef FLAG_H
#define FLAG_H

#include "cad.h"
#include "mesh.h"

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

typedef struct mflag mflag;

mflag* mflag_new(mesh* m);
void mflag_grow(mflag* f, simplex t, unsigned c);
void mflag_clear(mflag* f, ment e);
void mflag_set(mflag* f, ment e);
int mflag_get(mflag* f, ment e);
void mflag_free(mflag* f);
unsigned mflag_count(mflag* f, simplex t);
mflag* mflag_new_all(mesh* m, simplex t);

#endif
