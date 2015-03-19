#include "mesh_geom.h"

line ment_line(mesh* m, ment e)
{
  ment v[2];
  ment_verts(m, e, v);
  return verts_line(m, v);
}

triangle ment_triangle(mesh* m, ment e)
{
  ment v[3];
  ment_verts(m, e, v);
  return verts_triangle(m, v);
}

tet ment_tet(mesh* m, ment e)
{
  ment v[4];
  ment_verts(m, e, v);
  return verts_tet(m, v);
}

line verts_line(mesh* m, ment const v[])
{
  return line_new(mesh_point(m, v[0]), mesh_point(m, v[1]));
}

triangle verts_triangle(mesh* m, ment const v[])
{
  triangle t;
  t.a = mesh_point(m, v[0]);
  t.b = mesh_point(m, v[1]);
  t.c = mesh_point(m, v[2]);
  return t;
}

tet verts_tet(mesh* m, ment const v[])
{
  tet t;
  t.a = mesh_point(m, v[0]);
  t.b = mesh_point(m, v[1]);
  t.c = mesh_point(m, v[2]);
  t.d = mesh_point(m, v[3]);
  return t;
}

