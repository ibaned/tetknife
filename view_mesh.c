#include "view_mesh.h"
#include "mesh_adj.h"
#include "mesh_geom.h"

static void view_elem_faces(view* v, mesh* m, ment e, color c)
{
  unsigned i;
  ment fv[3];
  for (i = 0; i < simplex_ndown[e.t][TRIANGLE]; ++i) {
    mesh_down(m, e, TRIANGLE, i, fv);
    view_triangle(v, verts_triangle(m, fv), c);
  }
}

static void view_elem_edges(view* v, mesh* m, ment e, color c)
{
  unsigned ne;
  unsigned i;
  ment ev[2];
  ne = simplex_ndown[e.t][EDGE];
  for (i = 0; i < ne; ++i) {
    mesh_down(m, e, EDGE, i, ev);
    view_line(v, verts_line(m, ev), c);
  }
}

static void view_mesh_edges(view* v, mesh* m, color c)
{
  ment e;
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    view_elem_edges(v, m, e, c);
}

void view_mesh_wireframe(view* v, mesh* m)
{
  view_clear(v, black);
  view_mesh_edges(v, m, green);
}

static void view_mesh_faces(view* v, mesh* m, color c)
{
  ment e;
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    view_elem_faces(v, m, e, c);
}

void view_mesh_parts(view* v, mesh* m, color c)
{
  view_clear(v, black);
  view_mesh_faces(v, m, c);
  view_mesh_edges(v, m, white);
}
