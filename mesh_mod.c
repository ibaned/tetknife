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

struct collapse {
  mesh* m;
  mset oe[SIMPLICES];
  mset ke[SIMPLICES];
  mset ce[SIMPLICES];
  mset ne[SIMPLICES];
  ment v[2];
};

typedef enum {
  DONT_CHECK_DUPLICATES,
  CHECK_DUPLICATES
} check_duplicates;

static ment rebuild(mesh* m, ment e, ment ov, ment nv,
    check_duplicates co)
{
  ment v[SIMPLEX_MAX_DOWN];
  unsigned n, i;
  ment ne;
  n = ment_verts(m, e, v);
  for (i = 0; i < n; ++i)
    if (ment_eq(v[i], ov))
      v[i] = nv;
  if (co == CHECK_DUPLICATES) {
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

static void set_sv(split* s, ment* sv)
{
  if (ment_ok(*sv)) {
    s->sv = *sv;
    s->vf = GIVEN_VERT;
  } else {
    s->sv = ment_new(s->m, VERTEX, 0);
    *sv = s->sv;
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
  ment er;
  set_sv(s, &sv);
  if (s->vf == MADE_VERT)
    mesh_transfer_point(s->m, t, v, sv);
  all_up(s->m, t, v, s->oe);
  if (s->vf == MADE_VERT)
    classif_transfer_vert(s->m, sv, s->oe);
  nv = simplex_ndown[t][VERTEX];
  for (rt = t; rt < SIMPLICES; ++rt) {
    mset_reserve(s->ne + rt, s->oe[rt].s.n * nv);
    mset_clear(s->ne + rt);
    for (i = 0; i < s->oe[rt].s.n; ++i)
      for (j = 0; j < nv; ++j) {
        er = rebuild(s->m, s->oe[rt].e[i], v[j], sv, DONT_CHECK_DUPLICATES);
        mset_add(s->ne + rt, er);
      }
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
  return mset_min_quality(s->m, s->ne + mesh_elem(s->m));
}

void split_edge(split* s, ment v[2])
{
  split_start(s, EDGE, v, ment_null);
  split_accept(s);
}

collapse* collapse_new(mesh* m)
{
  collapse* c;
  simplex t;
  c = my_malloc(sizeof(*c));
  c->m = m;
  for (t = 0; t < SIMPLICES; ++t) {
    mset_init(c->oe + t);
    mset_init(c->ke + t);
    mset_init(c->ce + t);
    mset_init(c->ne + t);
  }
  return c;
}

void collapse_free(collapse* c)
{
  simplex t;
  for (t = 0; t < SIMPLICES; ++t) {
    mset_dtor(c->oe + t);
    mset_dtor(c->ke + t);
    mset_dtor(c->ce + t);
    mset_dtor(c->ne + t);
  }
  my_free(c);
}

void collapse_start_from(collapse* c, ment v)
{
  all_up(c->m, VERTEX, &v, c->oe);
  c->v[0] = v;
}

static int check_classif(collapse* c)
{
  gent gv[2];
  gent ge;
  if (!mesh_classif(c->m))
    return 1;
  gv[0] = classif_get(c->m, c->v[0]);
  gv[1] = classif_get(c->m, c->v[1]);
  if (gv[0].t < gv[1].t)
    return 0;
  ge = classif_by_adj(c->m, c->ce);
  return gent_eq(gv[0], ge);
}

static int check_pull(collapse* c)
{
  return c->ke[mesh_elem(c->m)].s.n != 0;
}

static int check_geom(collapse* c)
{
  simplex et;
  unsigned i;
  et = mesh_elem(c->m);
  for (i =0; i < c->ne[et].s.n; ++i)
    if (ment_size(c->m, c->ne[et].e[i]) <= 0)
      return 0;
  if (et != TRIANGLE)
    return 1;
  for (i =0; i < c->ne[et].s.n; ++i) {
    point on = triangle_norm(ment_triangle(c->m, c->ke[et].e[i]));
    point nn = triangle_norm(ment_triangle(c->m, c->ne[et].e[i]));
    if (point_dot(on, nn) <= 0)
      return 0;
  }
  return 1;
}

int collapse_start_to(collapse* c, ment v)
{
  simplex t;
  unsigned i;
  ment er;
  c->v[1] = v;
  all_up(c->m, EDGE, c->v, c->ce);
  if (!check_classif(c))
    return 0;
  for (t = 1; t < SIMPLICES; ++t)
    mset_sub(c->oe + t, c->ce + t, c->ke + t);
  if (!check_pull(c))
    return 0;
  for (t = 1; t < SIMPLICES; ++t) {
    mset_clear(c->ne + t);
    mset_reserve(c->ne + t, c->ke[t].s.n);
    for (i = 0; i < c->ke[t].s.n; ++i) {
      er = rebuild(c->m, c->ke[t].e[i], c->v[0], c->v[1], CHECK_DUPLICATES);
      if (!ment_ok(er))
        return 0;
      mset_add(c->ne + t, er);
    }
  }
  if (!check_geom(c))
    return 0;
  return 1;
}

void collapse_accept(collapse* c)
{
  rm_sets(c->m, c->oe);
  ment_free(c->m, c->v[0]);
}

void collapse_cancel(collapse* c)
{
  rm_sets(c->m, c->ne);
}

double collapse_quality(collapse* c)
{
  return mset_min_quality(c->m, c->ne + mesh_elem(c->m));
}

void collapse_edge(collapse* c, ment v[2])
{
  collapse_start_from(c, v[0]);
  ASSERT(collapse_start_to(c, v[1]));
  collapse_accept(c);
}
