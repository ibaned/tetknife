#include "../luby.h"
#include "../comm.h"
#include "../basics.h"

int main()
{
  unsigned nneigh;
  int neigh[2];
  unsigned color;
  mpi_init();
  comm_init(mpi_world());
  neigh[0] = (comm_rank() + (comm_size() - 1)) % comm_size();
  neigh[1] = (comm_rank() + 1) % comm_size();
  nneigh = (unsigned)(comm_size() - 1);
  nneigh = MIN(nneigh, 2);
  color = luby_color(nneigh, neigh);
  print("%d colored %d\n", comm_rank(), color);
  comm_finalize();
  mpi_finalize();
  return 0;
}
