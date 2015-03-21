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

line vfield_verts_line(vfield* f, ment const v[])
{
  line l;
  l.a = vfield_get(f, v[0]);
  l.b = vfield_get(f, v[1]);
  return l;
}

triangle vfield_verts_triangle(vfield* f, ment const v[])
{
  triangle t;
  t.a = vfield_get(f, v[0]);
  t.b = vfield_get(f, v[1]);
  t.c = vfield_get(f, v[2]);
  return t;
}

tet vfield_verts_tet(vfield* f, ment const v[])
{
  tet t;
  t.a = vfield_get(f, v[0]);
  t.b = vfield_get(f, v[1]);
  t.c = vfield_get(f, v[2]);
  t.d = vfield_get(f, v[3]);
  return t;
}

line vfield_line(vfield* f, ment e)
{
  ment v[2];
  ment_verts(f->m, e, v);
  return vfield_verts_line(f, v);
}

triangle vfield_triangle(vfield* f, ment e)
{
  ment v[3];
  ment_verts(f->m, e, v);
  return vfield_verts_triangle(f, v);
}

tet vfield_tet(vfield* f, ment e)
{
  ment v[4];
  ment_verts(f->m, e, v);
  return vfield_verts_tet(f, v);
}
