#include "comm.h"
#include "ibarrier.h"
#include "basics.h"
#include "stack.h"

/* Hoefler, Torsten, Christian Siebert, and Andrew Lumsdaine.
   "Scalable communication protocols for dynamic sparse data exchange."
   ACM Sigplan Notices 45.5 (2010): 159-168. */

typedef struct {
  stack s;
  void* data;
  mpi_request* r;
  int peer;
  int padding_;
} msg;

typedef struct {
  msg* m;
  unsigned n;
  int padding_;
} msgs;

static msgs global_in;
static msgs global_out;
static unsigned global_idx;
static mpi* global_mpi;
static mpi* global_ibarrier_mpi;
static enum {
  UNINIT    = 0,
  IDLE      = 1,
  PACK      = 2,
  UNPACK    = 3,
  BOTHPACK  = 4
} global_state = UNINIT;

static void init_msgs(msgs* ms)
{
  ms->m = 0;
  ms->n = 0;
}

void comm_init(mpi* m)
{
  ASSERT(global_state == UNINIT);
  global_mpi = mpi_copy(m);
  global_ibarrier_mpi = mpi_copy(m);
  global_state = IDLE;
  init_msgs(&global_in);
  init_msgs(&global_out);
}

int comm_rank(void)
{
  return mpi_rank(global_mpi);
}

int comm_size(void)
{
  return mpi_size(global_mpi);
}

void comm_finalize(void)
{
  ASSERT(global_state == IDLE);
  mpi_free(global_mpi);
  mpi_free(global_ibarrier_mpi);
  global_state = UNINIT;
}

static msg* add_msg(msgs* ms)
{
  msg* m;
  ms->n++;
  REALLOC(ms->m, ms->n);
  m = ms->m + ms->n - 1;
  m->data = 0;
  stack_init(&m->s);
  return m;
}

static int compare_msgs(void const* a_, void const* b_)
{
  msg const* a = a_;
  msg const* b = b_;
  return a->peer - b->peer;
}

static void sort_msgs(msgs* ms)
{
  my_qsort(ms->m, ms->n, sizeof(msg), compare_msgs);
}

static msg* find_out_msg(int peer)
{
  msgs* ms;
  msg key;
  ms = &global_out;
  key.peer = peer;
  return my_bsearch(&key, ms->m, ms->n, sizeof(msg), compare_msgs);
}

static msg* get_out_msg(int peer)
{
  msg* m;
  m = find_out_msg(peer);
  if (m)
    return m;
  add_msg(&global_out)->peer = peer;
  sort_msgs(&global_out);
  return find_out_msg(peer);
}

void comm_pack(void* data, unsigned size, int to)
{
  msg* m;
  if (global_state == IDLE)
    global_state = PACK;
  else if (global_state == UNPACK)
    global_state = BOTHPACK;
  else
    ASSERT(global_state == PACK || global_state == BOTHPACK);
  if (!(0 <= to && to <= comm_size()))
    die("bad rank %d in comm_pack\n", to);
  m = get_out_msg(to);
  if (!stack_can_push(&m->s, size))
    m->data = my_realloc(m->data, stack_grow_by(&m->s, size));
  my_memcpy((char*)m->data + stack_push_by(&m->s, size), data, size);
}

static void free_msg(msg* m)
{
  my_free(m->data);
}

static void free_msgs(msgs* ms)
{
  unsigned i;
  for (i = 0; i < ms->n; ++i)
    free_msg(&ms->m[i]);
  my_free(ms->m);
  init_msgs(ms);
}

static int out_done(void)
{
  msgs* o;
  unsigned i;
  int done = 1;
  o = &global_out;
  for (i = 0; i < o->n; ++i) {
    if ((o->m[i].r) && mpi_done(o->m[i].r))
      o->m[i].r = 0;
    if (o->m[i].r)
      done = 0;
  }
  return done;
}

static void try_recv(void)
{
  msg m;
  if (mpi_recv_any(global_mpi, &m.data, &m.s.c, &m.peer)) {
    stack_clear(&m.s);
    *add_msg(&global_in) = m;
  }
}

void comm_exch(void)
{
  msgs* o;
  unsigned i;
  if (!(global_state == PACK || global_state == IDLE))
    die("%d called exch in state %d\n", comm_rank(), global_state);
  o = &global_out;
  for (i = 0; i < o->n; ++i)
    o->m[i].r = mpi_issend(global_mpi,
        o->m[i].data, o->m[i].s.n, o->m[i].peer);
  while (!out_done())
    try_recv();
  free_msgs(&global_out);
  ibarrier_begin(global_ibarrier_mpi);
  while (!ibarrier_done())
    try_recv();
  sort_msgs(&global_in);
  global_state = UNPACK;
  global_idx = 0;
}

static int unpacked(msg* m)
{
  return stack_full(&m->s);
}

int comm_recv(void)
{
  ASSERT(global_state == UNPACK || global_state == BOTHPACK);
  for (; global_idx < global_in.n; ++global_idx)
    if (!unpacked(global_in.m + global_idx))
      return 1;
  free_msgs(&global_in);
  if (global_state == UNPACK)
    global_state = IDLE;
  else
    global_state = PACK;
  return 0;
}

void comm_unpack(void* data, unsigned size)
{
  msg* m;
  ASSERT(global_state == UNPACK || global_state == BOTHPACK);
  m = &global_in.m[global_idx];
  my_memcpy(data, (char*)m->data + stack_push_by(&m->s, size), size);
}

int comm_from(void)
{
  return global_in.m[global_idx].peer;
}

