#ifndef FIELD_H
#define FIELD_H

#include "mesh.h"

typedef struct fields fields;

typedef struct {
  fields* fs;
  int i;
  int padding_;
} vfield;

fields* fields_new(mesh* m);
void fields_free(fields* fs);

void fields_grow(fields* fs, unsigned c);

vfield vfield_new(mesh* m);
void vfield_free(vfield f);

point vfield_get(vfield f, ment e);
void vfield_set(vfield f, ment e, point v);

vfield vfield_f(mesh* m);
vfield vfield_n(vfield f);
int vfield_ok(vfield f);

#endif
