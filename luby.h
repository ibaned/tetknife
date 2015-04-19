#ifndef MIS_LUBY_H
#define MIS_LUBY_H

#include "mesh.h"

int luby_mis(unsigned nneigh, int const neigh[], int in_Vp);
unsigned luby_color(unsigned nneigh, int const neigh[]);
unsigned luby_color_mesh_parts(mesh* m);

#endif
