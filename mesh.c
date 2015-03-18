#include "mesh.h"
#include "flex.h"
#include "basics.h"
#include "list.h"
#include "field.h"
#include "classif.h"

struct mesh {
  flex f[SIMPLICES];
  int* d[SIMPLICES];
  int* nu[SIMPLICES];
  int* fu[SIMPLICES];
  simplex et;
  int padding_;
  fields fs;
  vfield x;
  classif* cl;
};

ment const ment_null = { 0, NULL_IDX };
muse const muse_null = { 0, NULL_IDX };

int ment_eq(ment a, ment b)
{
  return a.t == b.t && a.i == b.i;
}

int ment_ok(ment e)
{
  return IDX_OK(e.i);
}

int muse_eq(muse a, muse b)
{
  return a.t == b.t && a.i == b.i;
}

int muse_ok(muse e)
{
  return IDX_OK(e.i);
}

mesh* mesh_new(void)
{
  mesh* m;
  simplex t;
  m = my_malloc(sizeof(*m));
  for (t = 0; t < SIMPLICES; ++t) {
    flex_init(&m->f[t]);
    m->d[t] = 0;
    m->nu[t] = 0;
    m->fu[t] = 0;
  }
  m->et = VERTEX;
  fields_init(&m->fs);
  vfield_init(m, &m->x);
  m->cl = 0;
  return m;
}

void mesh_free(mesh* m)
{
  simplex t;
  fields_dtor(m);
  if (m->cl)
    classif_free(m->cl);
  for (t = 0; t < SIMPLICES; ++t) {
    flex_dtor(&m->f[t]);
    my_free(m->d[t]);
    my_free(m->nu[t]);
    my_free(m->fu[t]);
  }
  my_free(m);
}

simplex mesh_element(mesh* m)
{
  return m->et;
}

static unsigned nverts(simplex t)
{
  return simplex_ndown[t][VERTEX];
}

static unsigned mesh_grow(mesh* m, simplex t)
{
  unsigned c;
  unsigned wc;
  simplex ut;
  c = flex_grow(&m->f[t]);
  if (t == VERTEX) {
    for (ut = EDGE; ut < m->et; ++ut)
      REALLOC(m->fu[ut], c);
    fields_grow(&m->fs, c);
  } else {
    wc = c * nverts(t);
    REALLOC(m->d[t], wc);
    REALLOC(m->nu[t], wc);
  }
  if (m->cl)
    classif_grow_ments(m->cl, t, c);
  return c;
}

static muse muse_at(ment e, unsigned i)
{
  muse u;
  u.t = e.t;
  u.i = (int)((unsigned)e.i * nverts(e.t) + i);
  return u;
}

static void raise_dim(mesh* m)
{
  REALLOC(m->fu[m->et], m->f[VERTEX].s.c);
  m->et++;
}

ment ment_new(mesh* m, simplex t, ment const v[])
{
  ment e;
  muse u;
  unsigned i;
  while (t < m->et)
    raise_dim(m);
  e.t = t;
  if (flex_full(&m->f[t]))
    mesh_grow(m, t);
  e.i = flex_add(&m->f[t]);
  if (t == VERTEX)
    return e;
  for (i = 0; i < nverts(t); ++i) {
    u = muse_at(e, i);
    ASSERT(v[i].t == VERTEX);
    m->d[u.t][u.i] = v[i].i;
    list_link(m->fu[t], m->nu[t], v[i].i, u.i);
  }
  return e;
}

void ment_free(mesh* m, ment e)
{
  muse u;
  int vi;
  unsigned i;
  flex_rm(&m->f[e.t], e.i);
  if (e.t == VERTEX)
    return;
  for (i = 0; i < nverts(e.t); ++i) {
    u = muse_at(e, i);
    vi = m->d[u.t][u.i];
    list_unlink(m->fu[e.t], m->nu[e.t], vi, u.i);
  }
}

ment ment_f(mesh* m, simplex t)
{
  ment e;
  e.t = t;
  e.i = flex_f(&m->f[t]);
  return e;
}

ment ment_n(mesh* m, ment e)
{
  e.i = flex_n(&m->f[e.t], e.i);
  return e;
}

unsigned ment_count(mesh* m, simplex t)
{
  return m->f[t].n;
}

unsigned ment_verts(mesh* m, ment e, ment v[])
{
  unsigned i;
  unsigned n;
  muse u;
  if (e.t == VERTEX) {
    v[0] = e;
    return 1;
  }
  n = nverts(e.t);
  for (i = 0; i < n; ++i) {
    u = muse_at(e, i);
    v[i].t = VERTEX;
    v[i].i = m->d[u.t][u.i];
  }
  return n;
}

muse muse_f(mesh* m, ment v, simplex t)
{
  muse u;
  ASSERT(v.t == VERTEX);
  u.t = t;
  u.i = m->fu[t][v.i];
  return u;
}

muse muse_n(mesh* m, muse u)
{
  u.i = m->nu[u.t][u.i];
  return u;
}

ment muse_of(muse u)
{
  ment e;
  e.t = u.t;
  e.i = (int)((unsigned)u.i / nverts(e.t));
  return e;
}

point mesh_point(mesh* m, ment v)
{
  return vfield_get(&m->x, v);
}

void mesh_set_point(mesh* m, ment v, point x)
{
  vfield_set(&m->x, v, x);
}

unsigned mesh_cap(mesh* m, simplex t)
{
  return m->f[t].s.c;
}

struct fields* mesh_fields(mesh* m)
{
  return &m->fs;
}

struct classif* mesh_classif(mesh* m)
{
  return m->cl;
}

void mesh_set_classif(mesh* m, struct classif* cl)
{
  m->cl = cl;
}
