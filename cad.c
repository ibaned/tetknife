#include "cad.h"
#include "flex.h"
#include "list.h"

typedef struct {
  flex e;
  int* bf;
  int* bl;
  int* uf;
  int* ul;
} gents;

typedef struct {
  flex b;
  int* bn;
  int* bp;
  int* e;
  int* uf;
  int* ul;
} gbnds;

typedef struct {
  flex u;
  int* en;
  int* ep;
  int* e;
  int* b;
  int* bn;
  int* bp;
} guses;

struct cad_struct {
  gents e[CAD_ENT_TYPES];
  gbnds b[CAD_BND_TYPES];
  guses u[CAD_USE_TYPES];
};

gent const gent_null = { 0, NULL_IDX };

int gent_eq(gent a, gent b)
{
  return a.t == b.t && a.i == b.i;
}

int gent_ok(gent e)
{
  return IDX_OK(e.i);
}

int gbnd_ok(gbnd b)
{
  return IDX_OK(b.i);
}

int guse_ok(guse u)
{
  return IDX_OK(u.i);
}

static void gents_init(gents* e)
{
  flex_init(&e->e);
  e->bf = 0;
  e->bl = 0;
  e->uf = 0;
  e->ul = 0;
}

static void gents_dtor(gents* e)
{
  flex_dtor(&e->e);
  my_free(e->bf);
  my_free(e->bl);
  my_free(e->uf);
  my_free(e->ul);
}

static int gents_full(gents* e)
{
  return flex_full(&e->e);
}

static void gents_grow(gents* e, gent_type t)
{
  unsigned c;
  c = flex_grow(&e->e);
  if (t != CAD_VERTEX) {
    REALLOC(e->bf, c);
    REALLOC(e->bl, c);
  }
  if (t != CAD_REGION) {
    REALLOC(e->uf, c);
    REALLOC(e->ul, c);
  }
}

static int gents_add(gents* e, gent_type t)
{
  int i;
  i = flex_add(&e->e);
  if (t != CAD_VERTEX) {
    e->bf[i] = NULL_IDX;
    e->bl[i] = NULL_IDX;
  }
  if (t != CAD_REGION) {
    e->uf[i] = NULL_IDX;
    e->ul[i] = NULL_IDX;
  }
  return i;
}

static void gents_rm(gents* e, int i)
{
  flex_rm(&e->e, i);
}

static void gbnds_init(gbnds* b)
{
  flex_init(&b->b);
  b->bn = 0;
  b->bp = 0;
  b->e = 0;
  b->uf = 0;
  b->ul = 0;
}

static void gbnds_dtor(gbnds* b)
{
  flex_dtor(&b->b);
  my_free(b->bn);
  my_free(b->bp);
  my_free(b->e);
  my_free(b->uf);
  my_free(b->ul);
}

static int gbnds_full(gbnds* b)
{
  return flex_full(&b->b);
}

static void gbnds_grow(gbnds* b)
{
  unsigned c;
  c = flex_grow(&b->b);
  REALLOC(b->bn, c);
  REALLOC(b->bp, c);
  REALLOC(b->e, c);
  REALLOC(b->uf, c);
  REALLOC(b->ul, c);
}

static int gbnds_add(gbnds* b)
{
  int i;
  i = flex_add(&b->b);
  b->bn[i] = NULL_IDX;
  b->bp[i] = NULL_IDX;
  b->e[i] = NULL_IDX;
  b->uf[i] = NULL_IDX;
  b->ul[i] = NULL_IDX;
  return i;
}

static void gbnds_rm(gbnds* b, int i)
{
  flex_rm(&b->b, i);
}

static void guses_init(guses* u)
{
  flex_init(&u->u);
  u->en = 0;
  u->ep = 0;
  u->e = 0;
  u->b = 0;
  u->bn = 0;
  u->bp = 0;
}

static void guses_dtor(guses* u)
{
  flex_dtor(&u->u);
  my_free(u->en);
  my_free(u->ep);
  my_free(u->e);
  my_free(u->b);
  my_free(u->bn);
  my_free(u->bp);
}

static int guses_full(guses* u)
{
  return flex_full(&u->u);
}

static void guses_grow(guses* u)
{
  unsigned c;
  c = flex_grow(&u->u);
  REALLOC(u->en, c);
  REALLOC(u->ep, c);
  REALLOC(u->e, c);
  REALLOC(u->b, c);
  REALLOC(u->bn, c);
  REALLOC(u->bp, c);
}

static int guses_add(guses* u)
{
  return flex_add(&u->u);
}

static void guses_rm(guses* u, int i)
{
  flex_rm(&u->u, i);
}

cad* cad_new(void)
{
  cad* c;
  gent_type et;
  gbnd_type bt;
  guse_type ut;
  c = my_malloc(sizeof(*c));
  for (et = 0; et < CAD_ENT_TYPES; ++et)
    gents_init(c->e + et);
  for (bt = 0; bt < CAD_BND_TYPES; ++bt)
    gbnds_init(c->b + bt);
  for (ut = 0; ut < CAD_USE_TYPES; ++ut)
    guses_init(c->u + ut);
  return c;
}

void cad_free(cad* c)
{
  gent_type et;
  gbnd_type bt;
  guse_type ut;
  for (et = 0; et < CAD_ENT_TYPES; ++et)
    gents_dtor(c->e + et);
  for (bt = 0; bt < CAD_BND_TYPES; ++bt)
    gbnds_dtor(c->b + bt);
  for (ut = 0; ut < CAD_USE_TYPES; ++ut)
    guses_dtor(c->u + ut);
  my_free(c);
}

gent gent_new(cad* c, gent_type t)
{
  gents* es;
  gent e;
  es = c->e + t;
  e.t = t;
  if (gents_full(es))
    gents_grow(es, t);
  e.i = gents_add(es, t);
  return e;
}

void gent_free(cad* c, gent e)
{
  guse u;
  gbnd b;
  for (u = guse_of_f(c, e); guse_ok(u); u = guse_of_n(c, u))
    guse_free(c, u);
  for (b = gbnd_of_f(c, e); gbnd_ok(b); b = gbnd_of_n(c, b))
    gbnd_free(c, b);
  gents_rm(c->e + e.t, e.i);
}

gent gent_f(cad* c, gent_type t)
{
  gent e;
  e.t = t;
  e.i = flex_f(&c->e[t].e);
  return e;
}

gent gent_n(cad* c, gent e)
{
  e.i = flex_n(&c->e[e.t].e, e.i);
  return e;
}

gbnd gbnd_new(cad* c, gbnd_type t)
{
  gbnds* bs;
  gbnd b;
  bs = c->b + t;
  b.t = t;
  if (gbnds_full(bs))
    gbnds_grow(bs);
  b.i = gbnds_add(bs);
  return b;
}

static gbnd_type const e2b[CAD_ENT_TYPES] = {
  CAD_BND_TYPES,
  CAD_PAIR,
  CAD_LOOP,
  CAD_SHELL
};

gbnd gbnd_of_new(cad* c, gent e)
{
  gbnd b;
  b = gbnd_new(c, e2b[e.t]);
  gbnd_of_set(c, b, e);
  return b;
}

void gbnd_free(cad* c, gbnd b)
{
  guse u;
  for (u = guse_by_f(c, b); guse_ok(u); u = guse_by_n(c, u))
    guse_free(c, u);
  gbnds_rm(c->b + b.t, b.i);
}

void gbnd_of_set(cad* c, gbnd b, gent e)
{
  ASSERT(b.t == e2b[e.t]);
  dlist_link(c->e[e.t].bf, c->e[e.t].bl, c->b[b.t].bn, c->b[b.t].bp, e.i, b.i);
  c->b[b.t].e[b.i] = e.i;
}

static gent_type const b2e[CAD_BND_TYPES] = {
  CAD_EDGE,
  CAD_FACE,
  CAD_REGION
};

gent gbnd_of(cad* c, gbnd b)
{
  gent e;
  e.t = b2e[b.t];
  e.i = c->b[b.t].e[b.i];
  return e;
}

gbnd gbnd_of_f(cad* c, gent e)
{
  gbnd b;
  b.t = e2b[e.t];
  b.i = c->e[e.t].bf[e.i];
  return b;
}

gbnd gbnd_of_n(cad* c, gbnd b)
{
  b.i = c->b[b.t].bn[b.i];
  return b;
}

static guse_type const e2u[CAD_ENT_TYPES] = {
  CAD_VERTEX_USE,
  CAD_EDGE_USE,
  CAD_FACE_USE,
  CAD_USE_TYPES
};

static guse_type const b2u[CAD_BND_TYPES] = {
  CAD_VERTEX_USE,
  CAD_EDGE_USE,
  CAD_FACE_USE
};

guse guse_new(cad* c, gent e, gbnd b)
{
  guses* us;
  guse u;
  u.t = b2u[b.t];
  ASSERT(u.t == e2u[e.t]);
  us = c->u + u.t;
  if (guses_full(us))
    guses_grow(us);
  u.i = guses_add(us);
  dlist_link(c->e[e.t].uf, c->e[e.t].ul, c->u[u.t].en, c->u[u.t].ep, e.i, u.i);
  dlist_link(c->b[b.t].uf, c->b[b.t].ul, c->u[u.t].bn, c->u[u.t].bp, b.i, u.i);
  c->u[u.t].e[u.i] = e.i;
  c->u[u.t].b[u.i] = b.i;
  return u;
}

void guse_free(cad* c, guse u)
{
  gent e;
  gbnd b;
  e = guse_of(c, u);
  b = guse_by(c, u);
  dlist_unlink(c->e[e.t].uf, c->e[e.t].ul, c->u[u.t].en, c->u[u.t].ep, e.i, u.i);
  dlist_unlink(c->b[b.t].uf, c->b[b.t].ul, c->u[u.t].bn, c->u[u.t].bp, b.i, u.i);
  guses_rm(c->u + e.t, e.i);
}

static gent_type const u2e[CAD_USE_TYPES] = {
  CAD_VERTEX,
  CAD_EDGE,
  CAD_FACE
};

static gbnd_type const u2b[CAD_USE_TYPES] = {
  CAD_PAIR,
  CAD_LOOP,
  CAD_SHELL
};

gent guse_of(cad* c, guse u)
{
  gent e;
  e.t = u2e[u.t];
  e.i = c->u[u.t].e[u.i];
  return e;
}

gbnd guse_by(cad* c, guse u)
{
  gbnd b;
  b.t = u2b[u.t];
  b.i = c->u[u.t].b[u.i];
  return b;
}

guse guse_of_f(cad* c, gent e)
{
  guse u;
  u.t = e2u[e.t];
  u.i = c->e[e.t].uf[e.i];
  return u;
}

guse guse_of_n(cad* c, guse u)
{
  u.i = c->u[u.t].en[u.i];
  return u;
}

guse guse_by_f(cad* c, gbnd b)
{
  guse u;
  u.t = b2u[b.t];
  u.i = c->b[b.t].uf[b.i];
  return u;
}

guse guse_by_n(cad* c, guse u)
{
  u.i = c->u[u.t].bn[u.i];
  return u;
}
