#ifndef FIELD_H
#define FIELD_H

#include "mesh.h"

typedef struct fields {
  struct vfield* vf;
} fields;

typedef struct vfield {
  mesh* m;
  struct vfield* n;
  point* x;
} vfield;

void fields_init(fields* fs);
void fields_dtor(mesh* m);

void fields_grow(fields* fs, unsigned c);

void vfield_init(mesh* m, vfield* f);
void vfield_dtor(vfield* f);

point vfield_get(vfield* f, ment e);
void vfield_set(vfield* f, ment e, point v);

line vfield_verts_line(vfield* f, ment const v[]);
triangle vfield_verts_triangle(vfield* f, ment const v[]);
tet vfield_verts_tet(vfield* f, ment const v[]);

line vfield_line(vfield* f, ment e);
triangle vfield_triangle(vfield* f, ment e);
tet vfield_tet(vfield* f, ment e);

#endif
