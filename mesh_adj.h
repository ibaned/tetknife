#ifndef MESH_ADJ_H
#define MESH_ADJ_H

#include "mesh.h"
#include "stack.h"

typedef struct mset {
  stack s;
  ment* e;
} mset;

void mset_init(mset* s);
void mset_dtor(mset* s);
void mset_clear(mset* s);
void mset_add(mset* s, ment e);
int mset_has(mset const* s, ment e);
void mset_reserve(mset* s, unsigned n);
void mset_sub(mset const* a, mset const* b, mset* c);

unsigned muse_count(mesh* m, ment v, simplex t);
int ments_have(unsigned n, ment const es[], ment e);
void mesh_up(mesh* m, simplex from, ment const dv[], simplex to, mset* s);
ment mesh_find(mesh* m, simplex t, ment const v[]);
int mesh_has(mesh* m, simplex t, ment const v[]);
unsigned mesh_down(mesh* m, ment e, simplex dt, unsigned di, ment dv[]);

#endif
