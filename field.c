#include "field.h"
#include "basics.h"

void fields_init(fields* fs)
{
  fs->vf = 0;
}

void fields_dtor(mesh* m)
{
  fields* fs = mesh_fields(m);
  while (fs->vf)
    vfield_dtor(fs->vf);
}

void fields_grow(fields* fs, unsigned c)
{
  vfield* v;
  for (v = fs->vf; v; v = v->n)
    REALLOC(v->x, c);
}

void vfield_init(mesh* m, vfield* f)
{
  fields* fs = mesh_fields(m);
  f->m = m;
  f->n = fs->vf;
  fs->vf = f;
  f->x = my_malloc(sizeof(point) * mesh_cap(m, VERTEX));
}

void vfield_dtor(vfield* f)
{
  fields* fs;
  vfield** p;
  fs = mesh_fields(f->m);
  for (p = &fs->vf; *p != f; *p = (*p)->n);
  *p = f->n;
  my_free(f->x);
}

point vfield_get(vfield* f, ment e)
{
  ASSERT(e.t == VERTEX);
  return f->x[e.i];
}

void vfield_set(vfield* f, ment e, point v)
{
  ASSERT(e.t == VERTEX);
  f->x[e.i] = v;
}
