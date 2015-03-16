#ifndef FIELD_H
#define FIELD_H

#include "mesh.h"

typedef struct fields {
  struct vfield* vf;
} fields;

typedef struct vfield {
  struct vfield* n;
  point* x;
} vfield;

void fields_init(fields* fs);
void fields_dtor(mesh* m);

void fields_grow(fields* fs, unsigned c);

void vfield_init(mesh* m, vfield* f);
void vfield_dtor(mesh* m, vfield* f);

point vfield_get(vfield* f, ment e);
void vfield_set(vfield* f, ment e, point v);

#endif
