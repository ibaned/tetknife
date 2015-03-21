#include "../view_mesh.h"
#include "../mesh_bbox.h"
#include "../mesh_adapt.h"

#define WIDTH 640
#define HEIGHT 480

int main()
{
  bbox b = {{0,0,0},{1,1,1}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  mesh* m;
  view* v;
  unsigned i;
  v = view_new(WIDTH, HEIGHT);
  m = mesh_new();
  mesh_gen_bbox(m, b, DIM3, bv, be);
  view_focus(v, mesh_bbox(m));
  view_mesh_wireframe(v, m);
  for (i = 0; i < 12; ++i) {
    mesh_refine_all(m);
    view_mesh_wireframe(v, m);
  }
  mesh_free(m);
  view_free(v);
  return 0;
}
