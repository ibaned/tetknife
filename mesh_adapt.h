#ifndef MESH_ADAPT_H
#define MESH_ADAPT_H

#include "mesh.h"
#include "flag.h"

void mesh_refine(mesh* m, mflag* f);
void mesh_refine_all(mesh* m);

#endif
