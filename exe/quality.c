#include "../simplex.h"
#include "../basics.h"
#include "../mesh_geom.h"

static void tri_test(void)
{
  mesh* m;
  point pts[3];
  ment v[3];
  ment e;
  unsigned i;
  m = mesh_new();
  pts[0] = point_new(-1,0,0);
  pts[1] = point_new( 1,0,0);
  pts[2] = point_new( 0,my_sqrt(3),0);
  for (i = 0; i < 3; ++i)
    v[i] = ment_new(m, VERTEX, 0);
  for (i = 0; i < 3; ++i)
    mesh_set_point(m, v[i], pts[i]);
  e = ment_new(m, TRIANGLE, v);
  debug("area: %e\n", triangle_area(ment_triangle(m, e)));
  mesh_free(m);
}

int main()
{
  tri_test();
  return 0;
}
