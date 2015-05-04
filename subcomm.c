#include "subcomm.h"
#include "remotes.h"
#include "basics.h"

void switch_comm(mpi* m)
{
  comm_finalize();
  comm_init(m);
}

static void mesh_regroup(mesh* m, int newgroup, int newrank)
{
  rpeer rp;
  int rrank;
  int othergroup;
  int otherrank;
  for (rp = rpeer_f(m); rpeer_ok(rp); rp = rpeer_n(m, rp)) {
    rrank = rpeer_rank(m, rp);
    COMM_PACK(newgroup, rrank);
    COMM_PACK(newrank, rrank);
  }
  comm_exch();
  while (comm_recv()) {
    COMM_UNPACK(othergroup);
    ASSERT(othergroup == newgroup);
    COMM_UNPACK(otherrank);
    rpeer_set_rank(m, rpeer_by_rank(m, comm_from()), otherrank);
  }
}

mpi* enter_groups(int newgroup, int newrank)
{
  mpi* oldcomm;
  mpi* subcomm;
  oldcomm = mpi_copy(comm_mpi());
  subcomm = mpi_split(oldcomm, newgroup, newrank);
  switch_comm(subcomm);
  return oldcomm;
}

void exit_groups(mpi* oldcomm)
{
  switch_comm(oldcomm);
  mpi_free(oldcomm);
}

mpi* mesh_enter_groups(mesh* m, int newgroup, int newrank)
{
  mpi* oldcomm;
  mesh_regroup(m, newgroup, newrank);
  oldcomm = enter_groups(newgroup, newrank);
  remotes_decide_owners(m);
  return oldcomm;
}

void mesh_exit_groups(mesh* m, mpi* oldcomm)
{
  mesh_regroup(m, 0, mpi_rank(oldcomm));
  exit_groups(oldcomm);
  remotes_decide_owners(m);
}

