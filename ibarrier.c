#include "ibarrier.h"

enum action {
  IDLE,
  SEND,
  RECV
};

static mpi* global_mpi;
static int global_bit;
static mpi_request* global_request;

static enum action (*global_action)(void);
static int (*global_peer)(void);
static int (*global_done)(void);
static void (*global_shift)(void);

static int reduce_peer(void)
{
  return mpi_rank(global_mpi) ^ global_bit;
}

static enum action reduce_action(void)
{
  if (reduce_peer() >= mpi_size(global_mpi))
    return IDLE;
  if (global_bit & mpi_rank(global_mpi))
    return SEND;
  return RECV;
}

static int reduce_done(void)
{
  int rank;
  rank = mpi_rank(global_mpi);
  if (rank==0)
    return global_bit >= mpi_size(global_mpi);
  return (global_bit >> 1) & rank;
}

static void reduce_shift(void)
{
  global_bit <<= 1;
}

static void reduce_begin(void)
{
  global_bit = 1;
  global_action = reduce_action;
  global_peer   = reduce_peer;
  global_done   = reduce_done;
  global_shift  = reduce_shift;
}

static int bcast_peer(void)
{
  return mpi_rank(global_mpi) ^ global_bit;
}

static enum action bcast_action(void)
{
  if (bcast_peer() >= mpi_size(global_mpi))
    return IDLE;
  if (global_bit & mpi_rank(global_mpi))
    return RECV;
  return SEND;
}

static int bcast_done(void)
{
  return global_bit == 0;
}

static void bcast_shift(void)
{
  global_bit >>= 1;
}

static int floor_log2(int n)
{
  int r = 0;
  while ((n >>= 1))
    ++r;
  return r;
}

static int ceil_log2(int n)
{
  int r = floor_log2(n);
  if ((1 << r) < n)
    ++r;
  return r;
}

static int bcast_first_bit(void)
{
  int rank;
  int bit;
  rank = mpi_rank(global_mpi);
  if (rank == 0)
    return 1 << ceil_log2(mpi_size(global_mpi));
  bit = 1;
  while ( ! (bit & rank))
    bit <<= 1;
  return bit;
}

static void bcast_begin(void)
{
  global_bit = bcast_first_bit();
  global_action = bcast_action;
  global_peer   = bcast_peer;
  global_done   = bcast_done;
  global_shift  = bcast_shift;
}

static void step_begin(void)
{
  enum action a;
  a = global_action();
  if (a == IDLE)
    return;
  else if (a == SEND)
    global_request = mpi_isend(global_mpi, 0, 0, global_peer());
  else if (a == RECV)
    global_request = mpi_irecv(global_mpi, 0, 0, global_peer());
}

static int step_done(void)
{
  enum action a;
  a = global_action();
  if (a == IDLE)
    return 1;
  if (!mpi_done(global_request))
    return 0;
  return 1;
}

static void coll_begin(void)
{
  if (global_done())
    return;
  step_begin();
}

static int coll_done(void)
{
  if (global_done())
    return 1;
  if (step_done()) {
    global_shift();
    if (global_done())
      return 1;
    step_begin();
  }
  return 0;
}

void ibarrier_begin(mpi* m)
{
  global_mpi = m;
  reduce_begin();
  coll_begin();
}

int ibarrier_done(void)
{
  if ((global_action == bcast_action) && coll_done())
    return 1;
  if ((global_action == reduce_action) && coll_done()) {
    bcast_begin();
    coll_begin();
  }
  return 0;
}

