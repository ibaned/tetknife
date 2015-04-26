#include "migrate.h"
#include "comm.h"
#include "remotes.h"
#include "mesh_adj.h"
#include "flag.h"

mlabel* migration_plan_new(mesh* m)
{
  mlabel* l;
  ment e;
  l = mlabel_new(m);
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    mlabel_set(l, e, comm_rank());
  return l;
}

static void pack_residence(mesh* m, mlabel* plan)
{
  ment v;
  rcopy o;
  unsigned ne;
  muse u;
  ment e;
  int rank;
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v)) {
    o = ment_owner(m, v);
    COMM_PACK(o.ri, o.rank);
    ne = muse_count(m, v, mesh_elem(m));
    COMM_PACK(ne, o.rank);
    for (u = muse_f(m, v, mesh_elem(m)); muse_ok(u); u = muse_n(m, u)) {
      e = muse_of(u);
      rank = (int) mlabel_get(plan, e);
      COMM_PACK(rank, o.rank);
    }
  }
}

static void pack_local(ment v, int to)
{
  ASSERT(v.i >= 0);
  COMM_PACK(v.i, to);
}

static rcopy unpack_remote(void)
{
  rcopy rc;
  COMM_UNPACK(rc.ri);
  rc.rank = comm_from();
  return rc;
}

static void pack_vertex(mesh* m, ment v, int to);

static void residence_to_new_verts(mesh* m)
{
  ment v;
  unsigned ne;
  unsigned i;
  rcopy rc;
  rc.ri = -1;
  v.t = VERTEX;
  while (comm_recv()) {
    COMM_UNPACK(v.i);
    COMM_UNPACK(ne);
    for (i = 0; i < ne; ++i) {
      COMM_UNPACK(rc.rank);
      if (!ment_shared_with(m, v, rc.rank)) {
        pack_vertex(m, v, rc.rank);
        pack_local(v, rc.rank);
        rent_new(m, v, rc);
      }
    }
  }
}

static void pack_common(mesh* m, ment e, int to)
{
  int fv;
  if (mesh_flag(m)) {
    fv = mflag_get(mesh_flag(m), e);
    COMM_PACK(fv, to);
  }
}

static void unpack_common(mesh* m, ment e)
{
  int fv;
  if (mesh_flag(m)) {
    COMM_UNPACK(fv);
    if (fv)
      mflag_set(mesh_flag(m), e);
    else
      mflag_clear(mesh_flag(m), e);
  }
}

static void pack_vertex(mesh* m, ment v, int to)
{
  point x;
  x = mesh_point(m, v);
  COMM_PACK(x, to);
  pack_common(m, v, to);
}

static ment unpack_vertex(mesh* m)
{
  point x;
  ment v;
  COMM_UNPACK(x);
  v = ment_new(m, VERTEX, 0);
  mesh_set_point(m, v, x);
  unpack_common(m, v);
  return v;
}

static void new_verts_to_reduce(mesh* m)
{
  rcopy rc;
  ment v;
  while (comm_recv()) {
    v = unpack_vertex(m);
    rc = unpack_remote();
    rent_new(m, v, rc);
    ment_set_owner(m, v, rc.rank);
    pack_remote(rc);
    pack_local(v, rc.rank);
  }
}

static void unpack_reduce(mesh* m)
{
  rcopy rc;
  ment v;
  while (comm_recv()) {
    v = unpack_local(VERTEX);
    rc = unpack_remote();
    rent_set_index(m, rent_by_rank(m, v, rc.rank), rc.ri);
  }
}

static void pack_bcast(mesh* m)
{
  ment v;
  unsigned nr;
  rent re;
  rcopy rc;
  rent re2;
  rcopy rc2;
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v)) {
    if (!ment_owned(m, v))
      continue;
    nr = rent_of_count(m, v);
    if (!nr)
      continue;
    for (re = rent_of_f(m, v); rent_ok(re); re = rent_of_n(m, re)) {
      rc = rent_copy(m, re);
      pack_remote(rc);
      COMM_PACK(nr, rc.rank);
      for (re2 = rent_of_f(m, v); rent_ok(re2); re2 = rent_of_n(m, re2)) {
        rc2 = rent_copy(m, re2);
        COMM_PACK(rc2.rank, rc.rank);
        COMM_PACK(rc2.ri, rc.rank);
      }
    }
  }
}

static void unpack_bcast(mesh* m)
{
  ment v;
  unsigned nr;
  unsigned i;
  rcopy rc;
  while (comm_recv()) {
    v = unpack_local(VERTEX);
    COMM_UNPACK(nr);
    for (i = 0; i < nr; ++i) {
      COMM_UNPACK(rc.rank);
      COMM_UNPACK(rc.ri);
      if (!ment_shared_with(m, v, rc.rank))
        rent_new(m, v, rc);
    }
  }
}

static void pack_ref(mesh* m, ment v, int to)
{
  rcopy rc;
  rc = rent_copy(m, rent_by_rank(m, v, to));
  pack_remote(rc);
}

static ment unpack_ref(void)
{
  return unpack_local(VERTEX);
}

static void pack_elem(mesh* m, ment e, int to)
{
  ment v[SIMPLEX_MAX_DOWN];
  unsigned nv;
  unsigned i;
  nv = ment_verts(m, e, v);
  COMM_PACK(e.t, to);
  for (i = 0; i < nv; ++i)
    pack_ref(m, v[i], to);
  pack_common(m, e, to);
}

static ment unpack_elem(mesh* m)
{
  ment e;
  ment v[SIMPLEX_MAX_DOWN];
  unsigned nv;
  unsigned i;
  COMM_UNPACK(e.t);
  ASSERT(e.t == mesh_elem(m));
  nv = simplex_ndown[e.t][VERTEX];
  for (i = 0; i < nv; ++i)
    v[i] = unpack_ref();
  e = ment_new(m, e.t, v);
  unpack_common(m, e);
  return e;
}

static void pack_and_free_elems(mesh* m, mlabel* plan)
{
  ment e;
  int rank;
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e)) {
    rank = (int) mlabel_get(plan, e);
    if (rank != comm_rank()) {
      pack_elem(m, e, rank);
      ment_free(m, e);
    }
  }
}

static void unpack_elems(mesh* m)
{
  while (comm_recv())
    unpack_elem(m);
}

static void pack_ment_rent_frees(mesh* m, ment v)
{
  rent re;
  for (re = rent_of_f(m, v); rent_ok(re); re = rent_of_n(m, re))
    pack_remote(rent_copy(m, re));
}

static void unpack_rent_free(mesh* m)
{
  ment v;
  rent re;
  v = unpack_local(VERTEX);
  ASSERT(v.i >= 0);
  re = rent_by_rank(m, v, comm_from());
  ASSERT(re.i >= 0);
  rent_free(m, re);
}

static void pack_rent_frees(mesh* m)
{
  ment v;
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v))
    if (!muse_ok(muse_f(m, v, mesh_elem(m))))
      pack_ment_rent_frees(m, v);
}

static void unpack_rent_frees(mesh* m)
{
  while (comm_recv())
    unpack_rent_free(m);
}

static void free_verts(mesh* m)
{
  ment v;
  rent re;
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v)) {
    if (muse_ok(muse_f(m, v, mesh_elem(m))))
      continue;
    while (rent_ok(re = rent_of_f(m, v)))
      rent_free(m, re);
    ment_free(m, v);
  }
}

void migrate(mesh* m, mlabel* plan)
{
  pack_residence(m, plan);
  comm_exch();
  residence_to_new_verts(m);
  comm_exch();
  new_verts_to_reduce(m);
  comm_exch();
  unpack_reduce(m);
  pack_bcast(m);
  comm_exch();
  unpack_bcast(m);
  pack_and_free_elems(m, plan);
  comm_exch();
  unpack_elems(m);
  pack_rent_frees(m);
  comm_exch();
  unpack_rent_frees(m);
  free_verts(m);
  remotes_decide_owners(m);
}
