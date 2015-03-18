#include "mesh_adj.h"

void mset_init(mset* s)
{
  stack_init(&s->s);
  s->e = 0;
}

void mset_dtor(mset* s)
{
  my_free(s->e);
}

void mset_clear(mset* s)
{
  stack_clear(&s->s);
}

void mset_add(mset* s, ment e)
{
  if (stack_full(&s->s))
    REALLOC(s->e, stack_grow(&s->s));
  s->e[s->s.n] = e;
  stack_push(&s->s);
}

int mset_has(mset* s, ment e)
{
  return ments_have(s->s.n, s->e, e);
}

void mset_reserve(mset* s, unsigned n)
{
  if (!stack_can_hold(&s->s, n))
    REALLOC(s->e, stack_grow_to(&s->s, n));
}

int ments_have(unsigned n, ment es[], ment e)
{
  unsigned i;
  for (i = 0; i < n; ++i)
    if (ment_eq(es[i], e))
      return 1;
  return 0;
}

static int ments_superset(unsigned nsup, ment sup[], unsigned nsub, ment sub[])
{
  unsigned i;
  for (i = 0; i < nsub; ++i)
    if (!ments_have(nsup, sup, sub[i]))
      return 0;
  return 1;
}

static int verts_bound_ment(mesh* m, simplex dt, ment dv[], ment e)
{
  ment uv[SIMPLEX_MAX_DOWN];
  unsigned nuv;
  unsigned ndv;
  nuv = ment_verts(m, e, uv);
  ndv = simplex_ndown[dt][VERTEX];
  return ments_superset(nuv, uv, ndv, dv);
}

void mesh_up(mesh* m, simplex from, ment dv[], simplex to, mset* s)
{
  muse u;
  mset_clear(s);
  for (u = muse_f(m, dv[0], to); muse_ok(u); u = muse_n(m, u))
    if (verts_bound_ment(m, from, dv, muse_of(u)))
      mset_add(s, muse_of(u));
}

ment mesh_find(mesh* m, simplex t, ment v[])
{
  muse u;
  for (u = muse_f(m, v[0], t); muse_ok(u); u = muse_n(m, u))
    if (verts_bound_ment(m, t, v, muse_of(u)))
      return muse_of(u);
  return ment_null;
}

int mesh_has(mesh* m, simplex t, ment v[])
{
  return ment_ok(mesh_find(m, t, v));
}
