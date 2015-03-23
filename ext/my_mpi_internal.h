#ifndef MY_MPI_INTERNAL_H
#define MY_MPI_INTERNAL_H

#include <mpi.h>
#include "../basics.h"

struct mpi {
  MPI_Comm comm;
};

struct mpi_request {
  MPI_Request r;
};

int mpi_done_internal(MPI_Request* r);

#define CALL(c) ASSERT(0 == (c))

#endif
