#ifndef MESH_MOD_H
#define MESH_MOD_H

#include "mesh.h"

typedef struct split split;
typedef struct collapse collapse;

split* split_new(mesh* m);
void split_free(split* s);
void split_start(split* s, simplex t, ment v[], ment sv);
void split_accept(split* s);
void split_cancel(split* s);
double split_quality(split* s);
void split_edge(split* s, ment v[2]);

#endif
