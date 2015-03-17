#include "classif.h"
#include "list.h"
#include "field.h"
#include "basics.h"

struct classif {
  mesh* m;
  cad* c;
  gent* e[SIMPLICES];
  int* f[CAD_ENT_TYPES][SIMPLICES];
  int* l[CAD_ENT_TYPES][SIMPLICES];
  int* n[SIMPLICES];
  int* p[SIMPLICES];
  vfield x;
};

classif* classif_new(mesh* m, cad* c)
{
  classif* cl;
  gent_type gt;
  simplex et;
  ASSERT(!mesh_classif(m));
  cl = my_malloc(sizeof(*cl));
  cl->m = m;
  cl->c = c;
  for (gt = 0; gt < CAD_ENT_TYPES; ++gt)
    for (et = 0; et < SIMPLICES; ++et)
      if (simplex_dim[et] <= gent_dim[gt]) {
        cl->f[gt][et] = my_malloc(sizeof(int) * gent_cap(c, gt));
        cl->l[gt][et] = my_malloc(sizeof(int) * gent_cap(c, gt));
      }
  for (et = 0; et < SIMPLICES; ++et) {
    cl->e[et] = 0;
    cl->n[et] = 0;
    cl->p[et] = 0;
  }
  vfield_init(m, &cl->x);
  mesh_set_classif(m, cl);
  return cl;
}

void classif_free(classif* cl)
{
  gent_type gt;
  simplex et;
  for (gt = 0; gt < CAD_ENT_TYPES; ++gt)
    for (et = 0; et < SIMPLICES; ++et)
      if (simplex_dim[et] <= gent_dim[gt]) {
        my_free(cl->f[gt][et]);
        my_free(cl->l[gt][et]);
      }
  for (et = 0; et < SIMPLICES; ++et) {
    my_free(cl->e[et]);
    my_free(cl->n[et]);
    my_free(cl->p[et]);
  }
  vfield_dtor(cl->m, &cl->x);
  mesh_set_classif(cl->m, 0);
  my_free(cl);
}

mesh* classif_mesh(classif* cl)
{
  return cl->m;
}

cad* classif_cad(classif* cl)
{
  return cl->c;
}

gent classif_get(mesh* m, ment me)
{
  return mesh_classif(m)->e[me.t][me.i];
}

void classif_set(mesh* m, ment me, gent ge)
{
  classif* cl;
  cl = mesh_classif(m);
  cl->e[me.t][me.i] = ge;
  dlist_link(cl->f[ge.t][me.t], cl->l[ge.t][me.t],
      cl->n[me.t], cl->p[me.t], ge.i, me.i);
}

void classif_rm(mesh* m, ment me)
{
  classif* cl;
  gent ge;
  cl = mesh_classif(m);
  ge = cl->e[me.t][me.i];
  dlist_unlink(cl->f[ge.t][me.t], cl->l[ge.t][me.t],
      cl->n[me.t], cl->p[me.t], ge.i, me.i);
}

point classif_point(mesh* m, ment me)
{
  return vfield_get(&(mesh_classif(m)->x), me);
}

void classif_set_point(mesh* m, ment me, point x)
{
  vfield_set(&(mesh_classif(m)->x), me, x);
}

ment classif_f(mesh* m, gent ge, simplex t)
{
  ment me;
  me.t = t;
  me.i = mesh_classif(m)->f[ge.t][t][ge.i];
  return me;
}

ment classif_n(mesh* m, ment me)
{
  me.i = mesh_classif(m)->n[me.t][me.i];
  return me;
}

void classif_grow_ments(classif* cl, simplex t, unsigned c)
{
  REALLOC(cl->e[t], c);
  REALLOC(cl->n[t], c);
  REALLOC(cl->p[t], c);
}
