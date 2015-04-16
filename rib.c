#include "rib.h"
#include "basics.h"
#include "comm.h"
#include "migrate.h"
#include "mesh_geom.h"

static point center_of_mass(unsigned n, point o[])
{
  point com;
  unsigned i;
  unsigned long tn;
  com = point_zero;
  for (i = 0; i < n; ++i)
    com = point_add(com, o[i]);
  mpi_add_doubles(comm_mpi(), point_arr(&com), 3);
  tn = mpi_add_ulong(comm_mpi(), n);
  return point_scale(com, 1.0 / tn);
}

/*
http://en.wikipedia.org/wiki/Moment_of_inertia#Angular_momentum
*/

static basis inertia_contrib(point o, point com)
{
  basis a;
  a = basis_cross(point_sub(o, com));
  return basis_cat(a, a);
}

static basis inertia_matrix(unsigned n, point o[], point com)
{
  basis a;
  unsigned i;
  a = basis_zero;
  for (i = 0; i < n; ++i)
    a = basis_add(a, inertia_contrib(o[i], com));
  mpi_add_doubles(comm_mpi(), point_arr(basis_arr(&a)), 9);
  return basis_scale(a, -1.0);
}

static point min_eigenvec(basis m)
{
  unsigned nw;
  double w[3];
  unsigned best;
  unsigned i;
  nw = basis_eigenvals(m, w);
  best = 0;
  for (i = 1; i < nw; ++i)
    if (ABS(w[i]) < ABS(w[best]))
      best = i;
  return basis_eigenvec(m, w[best]);
}

static unsigned count_local_in(unsigned n, point o[], plane p)
{
  unsigned i;
  unsigned nin = 0;
  for (i = 0; i < n; ++i)
    if (plane_has(p, o[i]))
      ++nin;
  return nin;
}

static unsigned long count_total_in(unsigned n, point o[], plane p)
{
  return mpi_add_ulong(comm_mpi(), count_local_in(n, o, p));
}

static plane median_plane(unsigned n, point o[], point axis)
{
  double d, min, max;
  plane p;
  unsigned i;
  unsigned long nin;
  unsigned long tn;
  static unsigned const maxiter = 60;
  min = max = point_dot(o[0], axis);
  for (i = 1; i < n; ++i) {
    d = point_dot(o[i], axis);
    min = MIN(min, d);
    max = MAX(max, d);
  }
  mpi_min_doubles(comm_mpi(), &min, 1);
  mpi_max_doubles(comm_mpi(), &max, 1);
  d   = (max - min) * (1.0 / 4.0 + epsilon);
  p.r = (max + min) / 2.0;
  p.n = axis;
  tn = mpi_add_ulong(comm_mpi(), n);
  for (i = 0; i < maxiter; ++i) {
    nin = count_total_in(n, o, p);
    if (nin == tn / 2)
      return p;
    if (nin < tn / 2)
      p.r -= d;
    else
      p.r += d;
    d /= 2.0;
  }
  die("median plane still not found after %u bisections\n", maxiter);
}

static void partition(unsigned* n, point** o, rcopy** rc, plane mp)
{
  unsigned pn;
  unsigned nn;
  point* po;
  point* no;
  rcopy* prc;
  rcopy* nrc;
  unsigned lin;
  unsigned long tin;
  unsigned lout;
  unsigned long tout;
  unsigned long in_i;
  unsigned long out_i;
  int rank_is_in;
  unsigned ranks_in;
  unsigned ranks_out;
  unsigned long quo;
  unsigned long rem;
  unsigned i;
  unsigned long dest_i;
  int dest_rank;
  unsigned rank, size;
  rank = (unsigned) comm_rank();
  size = (unsigned) comm_size();
  pn = *n;
  po = *o;
  prc = *rc;
  lin = count_local_in(pn, po, mp);
  lout = pn - lin;
  tin = mpi_add_ulong(comm_mpi(), lin);
  tout = mpi_add_ulong(comm_mpi(), lout);
  ranks_in = size / 2;
  rank_is_in = (rank < ranks_in);
  ranks_out = size - ranks_in;
  if (rank_is_in) {
    quo = tin / ranks_in;
    rem = tin % ranks_in;
    nn = (unsigned) quo;
    if (rank == ranks_in - 1)
      nn += (unsigned) rem;
  } else {
    quo = tout / ranks_out;
    rem = tout % ranks_out;
    nn = (unsigned) quo;
    if (rank - ranks_in == ranks_out - 1)
      nn += (unsigned) rem;
  }
  no = my_malloc(sizeof(point) * nn);
  nrc = my_malloc(sizeof(rcopy) * nn);
  in_i = mpi_exscan_ulong(comm_mpi(), lin);
  out_i = mpi_exscan_ulong(comm_mpi(), lout);
  for (i = 0; i < pn; ++i) {
    if (plane_has(mp, po[i])) {
      dest_i = in_i++;
      dest_rank = (int) (dest_i / quo);
      ASSERT(dest_rank < (int)ranks_in);
    } else {
      dest_i = out_i++;
      dest_rank = (int) (dest_i / quo);
      dest_rank += ranks_in;
      ASSERT((dest_rank - (int)ranks_in) < (int)ranks_out);
    }
    COMM_PACK(dest_i, dest_rank);
    COMM_PACK(po[i], dest_rank);
    COMM_PACK(prc[i], dest_rank);
  }
  comm_exch();
  while (comm_recv()) {
    COMM_UNPACK(dest_i);
    if (rank_is_in)
      i = (unsigned) (dest_i - (rank * quo));
    else
      i = (unsigned) (dest_i - ((rank - ranks_in) * quo));
    COMM_UNPACK(no[i]);
    ASSERT(plane_has(mp, no[i]) == rank_is_in);
    COMM_UNPACK(nrc[i]);
  }
  my_free(po);
  my_free(prc);
  *n = nn;
  *o = no;
  *rc = nrc;
}

static plane bisection_plane(unsigned n, point o[])
{
  point com = center_of_mass(n, o);
  basis im = inertia_matrix(n, o, com);
  point axis = min_eigenvec(im);
  return median_plane(n, o, axis);
}

static void inertial_bisection(unsigned* n, point** o, rcopy** idx)
{
  plane mp = bisection_plane(*n, *o);
  partition(n, o, idx, mp);
}

void recursive_inertial_bisection(unsigned* n, point** o, rcopy** idx)
{
  int osize;
  int orank;
  int size;
  mpi* ompi;
  mpi* submpi;
  osize = comm_size();
  orank = comm_rank();
  ompi = mpi_copy(comm_mpi());
  for (size = osize; size != 1; size /= 2) {
    ASSERT(size % 2 == 0);
    submpi = mpi_split(ompi, orank / size, orank % size);
    comm_finalize();
    comm_init(submpi);
    inertial_bisection(n, o, idx);
    mpi_free(submpi);
  }
  comm_finalize();
  comm_init(ompi);
  mpi_free(ompi);
}

static void prepare_rib_input(mesh* m, unsigned* n, point** o, rcopy** idx)
{
  ment e;
  unsigned i;
  *n = ment_count(m, mesh_elem(m));
  *o = my_malloc(sizeof(point) * (*n));
  *idx = my_malloc(sizeof(rcopy) * (*n));
  i = 0;
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e)) {
    (*o)[i] = ment_centroid(m, e);
    (*idx)[i].rank = comm_rank();
    (*idx)[i].ri = e.i;
  }
}

static mlabel* plan_from_rib(mesh* m, unsigned n, rcopy idx[])
{
  mlabel* plan;
  unsigned i;
  plan = migration_plan_new(m);
  for (i = 0; i < n; ++i)
    pack_remote(idx[i]);
  comm_exch();
  while (comm_recv())
    mlabel_set(plan, unpack_local(mesh_elem(m)), comm_from());
  return plan;
}

void mesh_balance_rib(mesh* m)
{
  unsigned n;
  point* o;
  rcopy* idx;
  mlabel* plan;
  prepare_rib_input(m, &n, &o, &idx);
  recursive_inertial_bisection(&n, &o, &idx);
  plan = plan_from_rib(m, n, idx);
  my_free(o);
  my_free(idx);
  migrate(m, plan);
  mlabel_free(plan);
}
