#include "../comm.h"
#include "../basics.h"

int main()
{
  const char* out0 = "one";
  const char* out1 = "two";
  char in[4];
  mpi_init();
  comm_init(mpi_world());
  print("yo, rank %d here\n", comm_rank());
  print("rep'n all %d of the world\n", comm_size());
  if (comm_rank() == 0)
    comm_pack(out0, 4, 1);
  else
    comm_pack(out1, 4, 0);
  comm_exch();
  while (comm_recv()) {
    comm_unpack(in, 4);
    print("%d got \"%s\" from %d\n",
        comm_rank(), in, comm_from());
  }
  comm_finalize();
  mpi_finalize();
  return 0;
}
