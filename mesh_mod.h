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

collapse* collapse_new(mesh* m);
void collapse_free(collapse* c);
void collapse_start_from(collapse* c, ment v);
int collapse_start_to(collapse* c, ment v);
void collapse_accept(collapse* c);
void collapse_cancel(collapse* c);
double collapse_quality(collapse* c);
void collapse_edge(collapse* c, ment v[2]);

#endif
