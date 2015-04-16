#include "my_mpi.h"
#include "basics.h"

void mpi_init(void)
{
}

void mpi_finalize(void)
{
}

mpi* mpi_world(void)
{
  return (mpi*)1;
}

mpi* mpi_copy(mpi* m)
{
  (void)m;
  return (mpi*)1;
}

mpi* mpi_split(mpi* m, int subgroup, int subrank)
{
  (void)m;
  (void)subgroup;
  (void)subrank;
  return (mpi*)1;
}

void mpi_free(mpi* m)
{
  (void)m;
}

int mpi_rank(mpi* m)
{
  (void)m;
  return 0;
}

int mpi_size(mpi* m)
{
  (void)m;
  return 1;
}

static void* global_data;
static unsigned global_size;
static enum { FULL, EMPTY } global_state = EMPTY;

mpi_request* mpi_isend(mpi* m, void* data, unsigned size, int to)
{
  (void)m;
  (void)to;
  ASSERT(global_state == EMPTY);
  global_data = data;
  global_size = size;
  global_state = FULL;
  return (mpi_request*)1;
}

mpi_request* mpi_issend(mpi* m, void* data, unsigned size, int to)
{
  return mpi_isend(m, data, size, to);
}

int mpi_recv_any(mpi* m, void** data, unsigned* size, int* from)
{
  (void)m;
  if (global_state == EMPTY)
    return 0;
  *data = my_malloc(global_size);
  my_memcpy(*data, global_data, global_size);
  *size = global_size;
  *from = 0;
  global_state = EMPTY;
  return 1;
}

mpi_request* mpi_irecv(mpi* m,
    void* data, unsigned size, int from)
{
  (void)m;
  (void)from;
  my_memcpy(data, global_data, size);
  return (mpi_request*)1;
}

int mpi_done(mpi_request* r)
{
  (void)r;
  return global_state == EMPTY;
}

void mpi_barrier(mpi* m)
{
  (void)m;
}

void mpi_add_doubles(mpi* m, double* x, unsigned n)
{
  (void)m;
  (void)x;
  (void)n;
}

void mpi_min_doubles(mpi* m, double* x, unsigned n)
{
  (void)m;
  (void)x;
  (void)n;
}

void mpi_max_doubles(mpi* m, double* x, unsigned n)
{
  (void)m;
  (void)x;
  (void)n;
}

unsigned long mpi_add_ulong(mpi* m, unsigned long x)
{
  (void)m;
  return x;
}

unsigned long mpi_exscan_ulong(mpi* m, unsigned long x)
{
  (void)m;
  (void)x;
  return 0;
}
