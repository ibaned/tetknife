#ifndef RIB_H
#define RIB_H

#include "space.h"
#include "remotes.h"

void recursive_inertial_bisection(unsigned* n, point** o, rcopy** idx);
void mesh_balance_rib(mesh* m);

#endif
