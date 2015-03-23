/* external header, nothing we can do */
#pragma clang diagnostic ignored "-Wlong-long"
#include "my_mpi_internal.h"

#include "../my_mpi.h"

void mpi_init(void)
{
  CALL(MPI_Init(0,0));
}

void mpi_finalize(void)
{
  CALL(MPI_Finalize());
}

mpi* mpi_world(void)
{
  static mpi world = { MPI_COMM_WORLD };
  return &world;
}

mpi* mpi_copy(mpi* m)
{
  mpi* d;
  d = my_malloc(sizeof(*d));
  CALL(MPI_Comm_dup(m->comm, &d->comm));
  return d;
}

mpi* mpi_split(mpi* m, int subgroup, int subrank)
{
  mpi* s;
  s = my_malloc(sizeof(*s));
  CALL(MPI_Comm_split(m->comm, subgroup, subrank, &s->comm));
  return s;
}

void mpi_free(mpi* m)
{
  ASSERT(m != mpi_world());
  CALL(MPI_Comm_free(&m->comm));
  my_free(m);
}

int mpi_rank(mpi* m)
{
  int rank;
  CALL(MPI_Comm_rank(m->comm, &rank));
  return rank;
}

int mpi_size(mpi* m)
{
  int size;
  CALL(MPI_Comm_size(m->comm, &size));
  return size;
}

mpi_request* mpi_isend(mpi* m, void* data, unsigned size, int to)
{
  mpi_request* r;
  r = my_malloc(sizeof(*r));
  CALL(MPI_Isend(data, (int)size, MPI_BYTE, to, 0, m->comm, &r->r));
  return r;
}

mpi_request* mpi_issend(mpi* m, void* data, unsigned size, int to)
{
  mpi_request* r;
  r = my_malloc(sizeof(*r));
  CALL(MPI_Issend(data, (int)size, MPI_BYTE, to, 0, m->comm, &r->r));
  return r;
}

int mpi_recv_any(mpi* m, void** data, unsigned* size, int* from)
{
  MPI_Status status;
  int flag;
  int count;
  CALL(MPI_Iprobe(MPI_ANY_SOURCE, 0, m->comm, &flag, &status));
  if (!flag)
    return 0;
  *from = status.MPI_SOURCE;
  CALL(MPI_Get_count(&status, MPI_BYTE, &count));
  *size = (unsigned)count;
  *data = my_malloc(*size);
  CALL(MPI_Recv(
        *data,
        count,
        MPI_BYTE,
        *from,
        0,
        m->comm,
        MPI_STATUS_IGNORE));
  return 1;
}

mpi_request* mpi_irecv(mpi* m, void* data, unsigned size, int from)
{
  mpi_request* r;
  r = my_malloc(sizeof(*r));
  CALL(MPI_Irecv(data, (int)size, MPI_BYTE, from, 0, m->comm, &r->r));
  return r;
}

int mpi_done_internal(MPI_Request* r)
{
  int flag;
  CALL(MPI_Test(r, &flag, MPI_STATUS_IGNORE));
  return flag;
}

int mpi_done(mpi_request* r)
{
  if (mpi_done_internal(&r->r)) {
    my_free(r);
    return 1;
  }
  return 0;
}

void mpi_barrier(mpi* m)
{
  CALL(MPI_Barrier(m->comm));
}
