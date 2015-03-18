#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "classif.h"

enum { MAX_BBOX_VERTS = 8 };
enum { MAX_BBOX_SIMPLICES = 6 };

extern unsigned const bbox_verts[DIMS];
extern unsigned const bbox_simplices[DIMS];

void mesh_gen_bbox(mesh* m, bbox b, dim d,
    ment v[MAX_BBOX_VERTS], ment e[MAX_BBOX_SIMPLICES]);
void classif_bbox_set(mesh* m, dim d, ment v[], ment e[], gent ge);

#endif
