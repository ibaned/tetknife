#ifndef MIS_LUBY_H
#define MIS_LUBY_H

#include "mesh.h"
#include "image.h"

int luby_mis(unsigned nneigh, int const neigh[], int in_V);
unsigned luby_color(unsigned nneigh, int const neigh[]);
unsigned luby_color_mesh_parts(mesh* m);

color color_from_hsv(double h, double s, double v);

color luby_color_from_index(unsigned ci);

#endif
