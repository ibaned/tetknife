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

void mesh_up(mesh* m, simplex from, ment dv[], simplex to, mset* s)
{
  muse u;
  ment e;
  ment uv[SIMPLEX_MAX_DOWN];
  unsigned nuv;
  unsigned ndv;
  ndv = simplex_ndown[from][VERTEX];
  mset_clear(s);
  for (u = muse_f(m, dv[0], to); muse_ok(u); u = muse_n(m, u)) {
    e = muse_of(u);
    nuv = ment_verts(m, e, uv);
    if (ments_superset(nuv, uv, ndv, dv))
      mset_add(s, e);
  }
}

