#include "mesh_geom.h"
#include "mesh_adj.h"

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

/* Shewchuk, J. "What is a good linear finite element?
   interpolation, conditioning, anisotropy, and quality measures (preprint)."
   University of California at Berkeley 73 (2002).

   http://www.cs.berkeley.edu/~jrs/papers/elem.pdf
 
   we choose the scale-invariant quality measures related to
   element matrix condition numbers for several reasons:
     1) they are simple, at least to state if not also to compute
     2) they are zero for zero-volume elements
     3) they are scale-invariant
 */

double triangle_quality(mesh* m, ment e)
{
  ment v[2];
  unsigned i;
  double A;
  double l[3];
  double s;
  A = triangle_area(ment_triangle(m, e));
  s = 0;
  for (i = 0; i < 3; ++i) {
    mesh_down(m, e, EDGE, i, v);
    l[i] = line_len(verts_line(m, v));
    s += l[i] * l[i];
  }
  s /= 3;
  return (A / s) / 4.330127e-01;
}

double tet_quality(mesh* m, ment e)
{
  ment v[3];
  unsigned i;
  double V;
  double A[4];
  double s;
  V = tet_volume(ment_tet(m, e));
  s = 0;
  for (i = 0; i < 4; ++i) {
    mesh_down(m, e, TRIANGLE, i, v);
    A[i] = triangle_area(verts_triangle(m, v));
    s += A[i] * A[i];
  }
  s /= 4;
  s = my_pow(s, 3.0 / 4.0);
  return (V / s) / 4.1360215960e-01;
}

double ment_quality(mesh* m, ment e)
{
  switch (e.t) {
    case VERTEX:
      return 1;
    case EDGE:
      return 1;
    case TRIANGLE:
      return triangle_quality(m, e);
    case TET:
      return tet_quality(m, e);
    case SIMPLICES:
      break;
  };
  die("bad ment_type %d in ment_quality\n", e.t);
}

double ment_size(mesh* m, ment e)
{
  switch (e.t) {
    case VERTEX:
      return 0;
    case EDGE:
      return line_len(ment_line(m, e));
    case TRIANGLE:
      return triangle_area(ment_triangle(m, e));
    case TET:
      return tet_volume(ment_tet(m, e));
    case SIMPLICES:
      break;
  };
  die("bad ment_type %d in ment_size\n", e.t);
}
