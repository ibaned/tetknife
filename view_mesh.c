#include "view_mesh.h"
#include "mesh_geom.h"

static void view_ment_edges(view* v, mesh* m, ment e)
{
  unsigned ne;
  unsigned i;
  ment ev[2];
  ne = simplex_ndown[e.t][EDGE];
  for (i = 0; i < ne; ++i) {
    mesh_down(m, e, EDGE, i, ev);
    view_line(v, verts_line(m, ev), green);
  }
}

void view_mesh_wireframe(view* v, mesh* m)
{
  simplex t;
  ment e;
  view_clear(v, black);
  for (t = EDGE; t < SIMPLICES; ++t)
    for (e = ment_f(m, t); ment_ok(e); e = ment_n(m, e))
      view_ment_edges(v, m, e);
}
