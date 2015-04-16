#include "../rib.h"
#include "../basics.h"
#include "../comm.h"

int main()
{
  unsigned n;
  point* o;
  rcopy* rc;
  int i;
  unsigned j;
  point allpts[8] = {
    { 1, 2,0},
    { 2, 1,0},
    { 1,-2,0},
    { 2,-1,0},
    {-1, 2,0},
    {-2, 1,0},
    {-1,-2,0},
    {-2,-1,0}
  };
  mpi_init();
  comm_init(mpi_world());
  n = 2;
  o = my_malloc(sizeof(point) * n);
  rc = my_malloc(sizeof(rcopy) * n);
  ASSERT(comm_size() == 4);
  if (comm_rank() == 0) {
    o[0] = allpts[3];
    o[1] = allpts[5];
  } else if (comm_rank() == 1) {
    o[0] = allpts[1];
    o[1] = allpts[4];
  } else if (comm_rank() == 2) {
    o[0] = allpts[6];
    o[1] = allpts[0];
  } else if (comm_rank() == 3) {
    o[0] = allpts[2];
    o[1] = allpts[7];
  }
  for (j = 0; j < n; ++j) {
    rc[j].rank = comm_rank();
    rc[j].ri = (int) j;
  }
  recursive_inertial_bisection(&n, &o, &rc);
  for (i = 0; i < comm_size(); ++i) {
    if (comm_rank() == i) {
      print("rank %d\n", i);
      print("post n = %u\n", n);
      for (j = 0; j < n; ++j) {
        print(" (%f,%f,%f)", o[j].x, o[j].y, o[j].z);
        print(" (%d,%d)", rc[j].rank, rc[j].ri);
        print("\n");
      }
    }
    mpi_barrier(comm_mpi());
  }
  my_free(o);
  my_free(rc);
  comm_finalize();
  mpi_finalize();
  return 0;
}
