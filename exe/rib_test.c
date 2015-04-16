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
  mpi_init();
  comm_init(mpi_world());
  n = 3;
  o = my_malloc(sizeof(point) * n);
  rc = my_malloc(sizeof(rcopy) * n);
  if (comm_rank() == 0) {
    o[0] = point_new(0,0,0);
    o[1] = point_new(1,1,0);
    o[2] = point_new(.3,.5,0);
  } else {
    o[0] = point_new(1,0,0);
    o[1] = point_new(0,1,0);
    o[2] = point_new(.5,.3,0);
  }
  for (j = 0; j < n; ++j) {
    rc[j].rank = comm_rank();
    rc[j].ri = (int) j;
  }
  inertial_bisection(&n, &o, &rc);
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
