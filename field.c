#include "field.h"
#include "flex.h"

struct fields {
  mesh* m;
  flex fv;
  point** v;
};

fields* fields_new(mesh* m)
{
  fields* fs;
  fs = my_malloc(sizeof(*fs));
  fs->m = m;
  flex_init(&fs->fv);
  fs->v = 0;
  mesh_set_fields(m, fs);
  return fs;
}

void fields_free(fields* fs)
{
  mesh_set_fields(fs->m, 0);
  while (vfield_ok(vfield_f(fs->m)))
    vfield_free(vfield_f(fs->m));
  my_free(fs);
}

void fields_grow(fields* fs, unsigned c)
{
  int i;
  for (i = flex_f(&fs->fv); IDX_OK(i); i = flex_n(&fs->fv, i))
    REALLOC(fs->v[i], c);
}

vfield vfield_new(mesh* m)
{
  vfield f;
  fields* fs = mesh_fields(m);
  f.fs = fs;
  if (flex_full(&fs->fv))
    REALLOC(fs->v, flex_grow(&fs->fv));
  f.i = flex_add(&fs->fv);
  fs->v[f.i] = 0;
  return f;
}

void vfield_free(vfield f)
{
  my_free(f.fs->v[f.i]);
  flex_rm(&f.fs->fv, f.i);
}

point vfield_get(vfield f, ment e)
{
  ASSERT(e.t == VERTEX);
  return f.fs->v[f.i][e.i];
}

void vfield_set(vfield f, ment e, point v)
{
  ASSERT(e.t == VERTEX);
  f.fs->v[f.i][e.i] = v;
}

vfield vfield_f(mesh* m)
{
  vfield f;
  f.fs = mesh_fields(m);
  f.i = flex_f(&f.fs->fv);
  return f;
}

vfield vfield_n(vfield f)
{
  f.i = flex_n(&f.fs->fv, f.i);
  return f;
}

int vfield_ok(vfield f)
{
  return IDX_OK(f.i);
}
