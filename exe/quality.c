#include "../simplex.h"
#include "../basics.h"
#include "../mesh_geom.h"
#include "../mesh_adj.h"

static void tri_test(void)
{
  mesh* m;
  point pts[3];
  ment v[3];
  ment e;
  unsigned i;
  double A;
  double l[3];
  double s;
  m = mesh_new();
  pts[0] = point_new(-1,0,0);
  pts[1] = point_new( 1,0,0);
  pts[2] = point_new( 0,my_sqrt(3),0);
  for (i = 0; i < 3; ++i)
    v[i] = ment_new(m, VERTEX, 0);
  for (i = 0; i < 3; ++i)
    mesh_set_point(m, v[i], pts[i]);
  e = ment_new(m, TRIANGLE, v);
  A = triangle_area(ment_triangle(m, e));
  debug("area: %e\n", A);
  s = 0;
  for (i = 0; i < 3; ++i) {
    mesh_down(m, e, EDGE, i, v);
    l[i] = line_len(verts_line(m, v));
    debug("length: %e\n", l[i]);
    s += l[i] * l[i];
  }
  s /= 3;
  debug("condition bound: %e\n", A / s);
  mesh_free(m);
}

static void tet_test(void)
{
  mesh* m;
  point pts[4];
  ment v[4];
  ment e;
  unsigned i;
  double V;
  double A[4];
  double s;
  m = mesh_new();
  pts[0] = point_new(-1, 0, -1.0 / my_sqrt(2));
  pts[1] = point_new( 1, 0, -1.0 / my_sqrt(2));
  pts[2] = point_new( 0,-1,  1.0 / my_sqrt(2));
  pts[3] = point_new( 0, 1,  1.0 / my_sqrt(2));
  for (i = 0; i < 4; ++i)
    v[i] = ment_new(m, VERTEX, 0);
  for (i = 0; i < 4; ++i)
    mesh_set_point(m, v[i], pts[i]);
  e = ment_new(m, TET, v);
  V = tet_volume(ment_tet(m, e));
  debug("volume: %e\n", V);
  s = 0;
  for (i = 0; i < 4; ++i) {
    mesh_down(m, e, TRIANGLE, i, v);
    A[i] = triangle_area(verts_triangle(m, v));
    debug("area: %e\n", A[i]);
    s += A[i] * A[i];
  }
  s /= 4;
  s = my_pow(s, 3.0 / 4.0);
  debug("condition bound: %e\n", V / s);
  mesh_free(m);
}

int main()
{
  tri_test();
  tet_test();
  return 0;
}
