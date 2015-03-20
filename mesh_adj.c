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

int ments_have(unsigned n, ment const es[], ment e)
{
  unsigned i;
  for (i = 0; i < n; ++i)
    if (ment_eq(es[i], e))
      return 1;
  return 0;
}

static int ments_superset(unsigned nsup, ment const sup[],
    unsigned nsub, ment const sub[])
{
  unsigned i;
  for (i = 0; i < nsub; ++i)
    if (!ments_have(nsup, sup, sub[i]))
      return 0;
  return 1;
}

static int verts_bound_ment(mesh* m, simplex dt, ment const dv[], ment e)
{
  ment uv[SIMPLEX_MAX_DOWN];
  unsigned nuv;
  unsigned ndv;
  nuv = ment_verts(m, e, uv);
  ndv = simplex_ndown[dt][VERTEX];
  /* assuming we already know that dv[0] is in the entity
                                     V       V    */
  return ments_superset(nuv, uv, ndv - 1, dv + 1);
}

void mesh_up(mesh* m, simplex from, ment const dv[], simplex to, mset* s)
{
  muse u;
  mset_clear(s);
  if (to < from)
    return;
  for (u = muse_f(m, dv[0], to); muse_ok(u); u = muse_n(m, u))
    if (verts_bound_ment(m, from, dv, muse_of(u)))
      mset_add(s, muse_of(u));
}

ment mesh_find(mesh* m, simplex t, ment const v[])
{
  muse u;
  for (u = muse_f(m, v[0], t); muse_ok(u); u = muse_n(m, u))
    if (verts_bound_ment(m, t, v, muse_of(u)))
      return muse_of(u);
  return ment_null;
}

int mesh_has(mesh* m, simplex t, ment const v[])
{
  return ment_ok(mesh_find(m, t, v));
}

static unsigned const vv0[1] = {0};
static unsigned const vv1[1] = {1};
static unsigned const vv2[1] = {2};
static unsigned const vv3[1] = {3};
static unsigned const* const vvv[1] = {vv0};
static unsigned const* const* const v_v[1] = {vvv};
static unsigned const* const evv[2] = {vv0,vv1};
static unsigned const eev0[2] = {0,1};
static unsigned const* const eev[1] = {eev0};
static unsigned const* const* const e_v[2] = {evv,eev};
static unsigned const* const fvv[3] = {vv0,vv1,vv2};
static unsigned const fev0[2] = {0,1};
static unsigned const fev1[2] = {1,2};
static unsigned const fev2[2] = {2,0};
static unsigned const* const fev[3] = {fev0,fev1,fev2};
static unsigned const ffv0[3] = {0,1,2};
static unsigned const* const ffv[1] = {ffv0};
static unsigned const* const* const f_v[3] = {fvv,fev,ffv};
static unsigned const* const rvv[4] = {vv0,vv1,vv2,vv3};
static unsigned const rev0[2] = {0,1};
static unsigned const rev1[2] = {1,2};
static unsigned const rev2[2] = {2,3};
static unsigned const rev3[2] = {3,0};
static unsigned const rev4[2] = {0,2};
static unsigned const rev5[2] = {1,3};
static unsigned const* const rev[6] = {rev0,rev1,rev2,rev3,rev4,rev5};
static unsigned const rfv0[3] = {0,2,1};
static unsigned const rfv1[3] = {0,3,2};
static unsigned const rfv2[3] = {0,1,3};
static unsigned const rfv3[3] = {1,2,3};
static unsigned const* const rfv[4] = {rfv0,rfv1,rfv2,rfv3};
static unsigned const rrv0[4] = {0,1,2,3};
static unsigned const* const rrv[1] = {rrv0};
static unsigned const* const* const r_v[4] = {rvv,rev,rfv,rrv};
static unsigned const* const* const* const down_table[4] = {v_v,e_v,f_v,r_v};

unsigned mesh_down(mesh* m, ment e, simplex dt, unsigned di, ment dv[])
{
  ment ev[SIMPLEX_MAX_DOWN];
  unsigned ndv;
  unsigned i;
  unsigned const* vi;
  ment_verts(m, e, ev);
  ndv = simplex_ndown[dt][VERTEX];
  vi = down_table[e.t][dt][di];
  for (i = 0; i < ndv; ++i)
    dv[i] = ev[vi[i]];
  return ndv;
}
