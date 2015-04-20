#include "../my_mpi.h"
#include "../basics.h"

#include <mpi.h>

struct mpi {
  MPI_Comm comm;
};

struct mpi_request {
  MPI_Request r;
};

#define CALL(c) ASSERT(0 == (c))

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

int mpi_done(mpi_request* r)
{
  int flag;
  CALL(MPI_Test(&r->r, &flag, MPI_STATUS_IGNORE));
  if (flag) {
    my_free(r);
    return 1;
  }
  return 0;
}

void mpi_barrier(mpi* m)
{
  CALL(MPI_Barrier(m->comm));
}

void mpi_add_doubles(mpi* m, double* x, unsigned n)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, x, (int)n, MPI_DOUBLE, MPI_SUM, m->comm));
}

void mpi_min_doubles(mpi* m, double* x, unsigned n)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, x, (int)n, MPI_DOUBLE, MPI_MIN, m->comm));
}

void mpi_max_doubles(mpi* m, double* x, unsigned n)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, x, (int)n, MPI_DOUBLE, MPI_MAX, m->comm));
}

unsigned long mpi_add_ulong(mpi* m, unsigned long x)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, &x, 1, MPI_UNSIGNED_LONG, MPI_SUM, m->comm));
  return x;
}

unsigned long mpi_exscan_ulong(mpi* m, unsigned long x)
{
  CALL(MPI_Exscan(MPI_IN_PLACE, &x, 1, MPI_UNSIGNED_LONG, MPI_SUM, m->comm));
  if (mpi_rank(m) == 0)
    return 0; /* wtf, mpi ?!?  rank 0 gets uninit data from exscan */
  return x;
}

int mpi_max_int(mpi* m, int x)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, &x, 1, MPI_INT, MPI_MAX, m->comm));
  return x;
}

unsigned mpi_max_unsigned(mpi* m, unsigned x)
{
  CALL(MPI_Allreduce(MPI_IN_PLACE, &x, 1, MPI_UNSIGNED, MPI_MAX, m->comm));
  return x;
}

static mpi_reduce_fn the_reduce_fn;

static void reduce_op(void* a, void* b, int* len, MPI_Datatype* dt)
{
  (void)len;
  (void)dt;
  the_reduce_fn(b, a);
}

void mpi_reduce(mpi* m, void* data, unsigned size, mpi_reduce_fn f)
{
  MPI_Datatype dt;
  MPI_Op op;
  MPI_Type_contiguous((int) size, MPI_BYTE, &dt);
  MPI_Type_commit(&dt);
  the_reduce_fn = f;
  MPI_Op_create(reduce_op, 1, &op);
  MPI_Reduce(MPI_IN_PLACE, data, 1, dt, op, 0, m->comm);
  MPI_Op_free(&op);
  MPI_Type_free(&dt);
}
