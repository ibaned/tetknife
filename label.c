#include "label.h"
#include "basics.h"

struct mlabel {
  long* v[SIMPLICES];
};

mlabel* mlabel_new(mesh* m)
{
  mlabel* l;
  simplex t;
  l = my_malloc(sizeof(*l));
  for (t = 0; t < SIMPLICES; ++t)
    l->v[t] = my_malloc(sizeof(long) * mesh_cap(m, t));
  return l;
}

void mlabel_free(mlabel* l)
{
  simplex t;
  for (t = 0; t < SIMPLICES; ++t)
    my_free(l->v[t]);
  my_free(l);
}

long mlabel_get(mlabel* l, ment e)
{
  return l->v[e.t][e.i];
}

void mlabel_set(mlabel* l, ment e, long v)
{
  l->v[e.t][e.i] = v;
}
