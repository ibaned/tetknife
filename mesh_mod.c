#include "mesh_mod.h"
#include "mesh_adj.h"
#include "classif.h"

struct split {
  mesh* m;
  mset oe[SIMPLICES];
  mset ne[SIMPLICES];
};

typedef enum {
  IGNORE_OVERLAP,
  CHECK_OVERLAP
} check_overlap;

static ment rebuild(mesh* m, ment e, ment ov, ment nv,
    check_overlap co)
{
  ment v[SIMPLEX_MAX_DOWN];
  unsigned n, i;
  ment ne;
  n = ment_verts(m, e, v);
  for (i = 0; i < n; ++i)
    if (ment_eq(v[i], ov))
      v[i] = nv;
  if (co == CHECK_OVERLAP) {
    ne = mesh_find(m, e.t, v);
    if (ment_ok(ne))
      return ment_null;
  }
  ne = ment_new(m, e.t, v);
  classif_transfer_rebuild(m, e, ne);
  return ne;
}

static void all_up(mesh* m, simplex t, ment v[], mset s[SIMPLICES])
{
  simplex ut;
  for (ut = 0; ut < SIMPLICES; ++ut)
    mesh_up(m, t, v, ut, s + ut);
}

void split_start_with(split* s, simplex t, ment v[], ment sv)
{
  unsigned nv;
  unsigned i, j;
  simplex rt;
  all_up(s->m, t, v, s->oe);
  nv = simplex_ndown[t][VERTEX];
  for (rt = t; rt < SIMPLICES; ++rt) {
    mset_reserve(s->ne + rt, s->oe[rt].s.n * nv);
    for (i = 0; i < s->oe[rt].s.n; ++i)
      for (j = 0; j < nv; ++j)
        mset_add(s->ne + rt, rebuild(
              s->m, s->oe[rt].e[i], v[i], sv, IGNORE_OVERLAP));
  }
}
