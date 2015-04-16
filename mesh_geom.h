#ifndef MESH_GEOM_H
#define MESH_GEOM_H

#include "mesh.h"
#include "mesh_adj.h"

line ment_line(mesh* m, ment e);
triangle ment_triangle(mesh* m, ment e);
tet ment_tet(mesh* m, ment e);

line verts_line(mesh* m, ment const v[]);
triangle verts_triangle(mesh* m, ment const v[]);
tet verts_tet(mesh* m, ment const v[]);

point verts_centroid(mesh* m, simplex t, ment const v[]);
point ment_centroid(mesh* m, ment e);

double triangle_quality(mesh* m, ment e);
double tet_quality(mesh* m, ment e);
double ment_quality(mesh* m, ment e);

double ment_size(mesh* m, ment e);

double mset_min_quality(mesh* m, struct mset* s);

void mesh_transfer_point(mesh* m, simplex t, ment const v[], ment sv);

#endif
