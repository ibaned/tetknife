#include "remotes.h"
#include "flex.h"
#include "comm.h"

struct ent {
  rent n;
  int ri;
  int li;
};

struct peer {
  int rank;
  int padding_;
  flex ef;
  struct ent* e;
};

struct remotes {
  mesh* m;
  rent* f;
  int* o;
  flex pf;
  struct peer* p;
};

#define RPEER_NULL { NULL_IDX }
static rpeer const rpeer_null = RPEER_NULL;
static rent const rent_null = { RPEER_NULL, NULL_IDX };

remotes* remotes_new(mesh* m)
{
  remotes* rs;
  ment v;
  rs = my_malloc(sizeof(*rs));
  rs->m = m;
  mesh_set_remotes(m, rs);
  rs->f = my_malloc(sizeof(rent) * mesh_cap(m, VERTEX));
  rs->o = my_malloc(sizeof(int) * mesh_cap(m, VERTEX));
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v))
    remotes_add_vert(m, v);
  flex_init(&rs->pf);
  rs->p = 0;
  return rs;
}

static void peer_free(remotes* rs, rpeer rp)
{
  struct peer* p;
  p = rs->p + rp.i;
  flex_dtor(&p->ef);
  my_free(p->e);
  flex_rm(&rs->pf, rp.i);
}

void remotes_free(remotes* rs)
{
  my_free(rs->f);
  my_free(rs->o);
  while (rpeer_ok(rpeer_f(rs->m)))
    peer_free(rs, rpeer_f(rs->m));
  my_free(rs->p);
  flex_dtor(&rs->pf);
  mesh_set_remotes(rs->m, 0);
  my_free(rs);
}

int rpeer_ok(rpeer p)
{
  return IDX_OK(p.i);
}

rpeer rpeer_f(mesh* m)
{
  rpeer rp;
  rp.i = flex_f(&mesh_remotes(m)->pf);
  return rp;
}

rpeer rpeer_n(mesh* m, rpeer rp)
{
  rp.i = flex_n(&mesh_remotes(m)->pf, rp.i);
  return rp;
}

rpeer rpeer_by_rank(mesh* m, int rank)
{
  rpeer rp;
  for (rp = rpeer_f(m); rpeer_ok(rp); rp = rpeer_n(m, rp))
    if (rpeer_rank(m, rp) == rank)
      return rp;
  return rpeer_null;
}

int rpeer_rank(mesh* m, rpeer rp)
{
  return mesh_remotes(m)->p[rp.i].rank;
}

int rent_ok(rent re)
{
  return IDX_OK(re.i);
}

int rent_eq(rent a, rent b)
{
  return a.p.i == b.p.i && a.i == b.i;
}

rent rent_f(mesh* m, rpeer rp)
{
  struct peer* p;
  rent re;
  re.p = rp;
  p = mesh_remotes(m)->p + rp.i;
  re.i = flex_f(&p->ef);
  return re;
}

rent rent_n(mesh* m, rent re)
{
  struct peer* p;
  p = mesh_remotes(m)->p + re.p.i;
  re.i = flex_n(&p->ef, re.i);
  return re;
}

rent rent_of_f(mesh* m, ment e)
{
  return mesh_remotes(m)->f[e.i];
}

rent rent_of_n(mesh* m, rent re)
{
  struct peer* p;
  p = mesh_remotes(m)->p + re.p.i;
  return p->e[re.i].n;
}

rcopy rent_copy(mesh* m, rent re)
{
  rcopy rc;
  struct peer* p;
  p = mesh_remotes(m)->p + re.p.i;
  rc.rank = rpeer_rank(m, re.p);
  rc.ri = p->e[re.i].ri;
  return rc;
}

ment rent_of(mesh* m, rent re)
{
  struct peer* p;
  ment v;
  v.t = VERTEX;
  p = mesh_remotes(m)->p + re.p.i;
  v.i = p->e[re.i].li;
  return v;
}

unsigned rent_of_count(mesh* m, ment e)
{
  rent re;
  unsigned n = 0;
  for (re = rent_of_f(m, e); rent_ok(re); re = rent_of_n(m, re))
    ++n;
  return n;
}

rent rent_by_rank(mesh* m, ment e, int rank)
{
  rent re;
  for (re = rent_of_f(m, e); rent_ok(re); re = rent_of_n(m, re))
    if (rpeer_rank(m, re.p) == rank)
      return re;
  return rent_null;
}

int ment_shared_with(mesh* m, ment e, int rank)
{
  return rent_ok(rent_by_rank(m, e, rank));
}

rent rent_new(mesh* m, ment me, rcopy rc)
{
  remotes* rs;
  rpeer rp;
  struct peer* p;
  rent re;
  struct ent* e;
  rs = mesh_remotes(m);
  rp = rpeer_by_rank(m, rc.rank);
  if (!rpeer_ok(rp)) {
    if (flex_full(&rs->pf))
      REALLOC(rs->p, flex_grow(&rs->pf));
    rp.i = flex_add(&rs->pf);
    p = rs->p + rp.i;
    p->rank = rc.rank;
    flex_init(&p->ef);
    p->e = 0;
  } else
    p = rs->p + rp.i;
  re.p = rp;
  if (flex_full(&p->ef))
    REALLOC(p->e, flex_grow(&p->ef));
  re.i = flex_add(&p->ef);
  e = p->e + re.i;
  e->n = rs->f[me.i];
  rs->f[me.i] = re;
  e->li = me.i;
  e->ri = rc.ri;
  return re;
}

void rent_free(mesh* m, rent re)
{
  remotes* rs;
  struct peer* p;
  ment v;
  rent* pre;
  rs = mesh_remotes(m);
  p = rs->p + re.p.i;
  v = rent_of(m, re);
  for (pre = &rs->f[v.i]; !rent_eq(*pre, re);
       pre = &(rs->p[pre->p.i].e[pre->i].n))
    *pre = rent_of_n(m, re);
  flex_rm(&p->ef, re.i);
  if (p->ef.n == 0)
    peer_free(rs, re.p);
}

rcopy ment_owner(mesh* m, ment e)
{
  int orank;
  rcopy rc;
  orank = mesh_remotes(m)->o[e.i];
  if (orank == comm_rank()) {
    rc.rank = orank;
    rc.ri = e.i;
  } else
    rc = rent_copy(m, rent_by_rank(m, e, orank));
  return rc;
}

void ment_set_owner(mesh* m, ment e, int rank)
{
  mesh_remotes(m)->o[e.i] = rank;
}

int ment_owned(mesh* m, ment e)
{
  return mesh_remotes(m)->o[e.i] == comm_rank();
}

void rent_set_index(mesh* m, rent re, int ri)
{
  mesh_remotes(m)->p[re.p.i].e[re.i].ri = ri;
}

void remotes_grow_verts(mesh* m, unsigned cap)
{
  remotes* rs;
  rs = mesh_remotes(m);
  REALLOC(rs->f, cap);
  REALLOC(rs->o, cap);
}

void remotes_add_vert(mesh* m, ment v)
{
  mesh_remotes(m)->f[v.i] = rent_null;
  mesh_remotes(m)->o[v.i] = comm_rank();
}
