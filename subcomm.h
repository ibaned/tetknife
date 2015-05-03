#ifndef SUBCOMM_H
#define SUBCOMM_H

#include "comm.h"
#include "mesh.h"

void switch_comm(mpi* m);
void mesh_regroup(mesh* m, int newgroup, int newrank);
mpi* enter_groups(int newgroup, int newrank);
void exit_groups(mpi* oldcomm);
mpi* mesh_enter_groups(mesh* m, int newgroup, int newrank);
void mesh_exit_groups(mesh* m, mpi* oldcomm);

#endif
