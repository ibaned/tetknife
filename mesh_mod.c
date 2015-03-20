#include "mesh_mod.h"
#include "mesh_adj.h"
#include "mesh_geom.h"
#include "classif.h"

typedef enum {
  MADE_VERT,
  GIVEN_VERT
} vert_from;

struct split {
  mesh* m;
  mset oe[SIMPLICES];
  mset ne[SIMPLICES];
  ment sv;
  vert_from vf;
  int padding_;
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

static void rm_sets(mesh* m, mset e[SIMPLICES])
{
  simplex t;
  unsigned i;
  for (t = 0; t < SIMPLICES; ++t)
    for (i = 0; i < e[t].s.n; ++i)
      ment_free(m, e[t].e[i]);
}

static void set_sv(split* s, ment sv)
{
  if (ment_ok(sv)) {
    s->sv = sv;
    s->vf = GIVEN_VERT;
  } else {
    s->sv = ment_new(s->m, VERTEX, 0);
    s->vf = MADE_VERT;
  }
}

split* split_new(mesh* m)
{
  split* s;
  simplex t;
  s = my_malloc(sizeof(*s));
  s->m = m;
  for (t = 0; t < SIMPLICES; ++t) {
    mset_init(s->oe + t);
    mset_init(s->ne + t);
  }
  return s;
}

void split_free(split* s)
{
  simplex t;
  for (t = 0; t < SIMPLICES; ++t) {
    mset_dtor(s->oe + t);
    mset_dtor(s->ne + t);
  }
  my_free(s);
}

void split_start(split* s, simplex t, ment v[], ment sv)
{
  unsigned nv;
  unsigned i, j;
  simplex rt;
  set_sv(s, sv);
  all_up(s->m, t, v, s->oe);
  if (s->vf == MADE_VERT)
    classif_transfer_vert(s->m, sv, s->oe);
  nv = simplex_ndown[t][VERTEX];
  for (rt = t; rt < SIMPLICES; ++rt) {
    mset_reserve(s->ne + rt, s->oe[rt].s.n * nv);
    for (i = 0; i < s->oe[rt].s.n; ++i)
      for (j = 0; j < nv; ++j)
        mset_add(s->ne + rt, rebuild(
              s->m, s->oe[rt].e[i], v[i], sv, IGNORE_OVERLAP));
  }
}

void split_accept(split* s)
{
  rm_sets(s->m, s->oe);
}

void split_cancel(split* s)
{
  rm_sets(s->m, s->ne);
  if (s->vf == MADE_VERT)
    ment_free(s->m, s->sv);
}

double split_quality(split* s)
{
  simplex t;
  for (t = SIMPLICES - 1; t > VERTEX; --t)
    if (s->ne[t].s.n)
      return mset_min_quality(s->m, s->ne + t);
  die("no sets in split_quality\n");
}

void split_edge(split* s, ment v[2])
{
  split_start(s, EDGE, v, ment_null);
  split_accept(s);
}
