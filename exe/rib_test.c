#include "../rib.h"
#include "../basics.h"
#include "../comm.h"

int main()
{
  unsigned n;
  point* o;
  rcopy* rc;
  unsigned i;
  mpi_init();
  comm_init(mpi_world());
  o = my_malloc(sizeof(point) * 2);
  rc = my_malloc(sizeof(rcopy) * 2);
  if (comm_rank() == 0) {
    o[0] = point_new(0,0,0);
    o[1] = point_new(1,1,0);
  } else {
    o[0] = point_new(1,0,0);
    o[1] = point_new(0,1,0);
  }
  rc[0].rank = comm_rank();
  rc[1].rank = comm_rank();
  rc[0].ri = 0;
  rc[1].ri = 0;
  inertial_bisection(&n, &o, &rc);
  print("post n = %u\n", n);
  for (i = 0; i < n; ++i) {
    print(" (%f,%f,%f)", o[i].x, o[i].y, o[i].z);
    print(" (%d,%d)", rc[i].rank, rc[i].ri);
  }
  comm_finalize();
  mpi_finalize();
  return 0;
}
