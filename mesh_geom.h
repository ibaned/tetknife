#ifndef MESH_GEOM_H
#define MESH_GEOM_H

#include "mesh.h"

line ment_line(mesh* m, ment e);
triangle ment_triangle(mesh* m, ment e);
tet ment_tet(mesh* m, ment e);

line verts_line(mesh* m, ment const v[]);
triangle verts_triangle(mesh* m, ment const v[]);
tet verts_tet(mesh* m, ment const v[]);

double triangle_quality(mesh* m, ment e);
double tet_quality(mesh* m, ment e);

#endif
