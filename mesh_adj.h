#ifndef MESH_ADJ_H
#define MESH_ADJ_H

#include "mesh.h"
#include "stack.h"

typedef struct {
  stack s;
  ment* e;
} mset;

void mset_init(mset* s);
void mset_dtor(mset* s);
void mset_clear(mset* s);
void mset_add(mset* s, ment e);
int mset_has(mset* s, ment e);

int ments_have(unsigned n, ment es[], ment e);
void mesh_up(mesh* m, simplex from, ment dv[], simplex to, mset* s);
ment mesh_find(mesh* m, simplex t, ment v[]);
int mesh_has(mesh* m, simplex t, ment v[]);

#endif
