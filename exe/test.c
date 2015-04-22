#include "../back.h"
#include "../view_mesh.h"
#include "../mesh_bbox.h"
#include "../mesh_adapt.h"
#include "../mesh_geom.h"
#include "../basics.h"

#define WIDTH 640
#define HEIGHT 480
static view* global_view;
static mesh* global_mesh;
static char global_key;

static void print_quality_stats(mesh* m)
{
  ment e;
  double q;
  double minq = 1;
  double maxq = 0;
  double sumq = 0;
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e)) {
    q = ment_quality(m, e);
    minq = MIN(minq, q);
    maxq = MAX(maxq, q);
    sumq += q;
  }
  print("quality stats:\n");
  print("minimum: %f\n", minq);
  print("maximum: %f\n", maxq);
  print("average: %f\n", sumq / ment_count(m, mesh_elem(m)));
}

static void render(void)
{
  view_mesh_wireframe(global_view, global_mesh);
}

void back_start(int argc, char** argv)
{
  bbox b = {{0,0,0},{1,1,1}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  (void)argc;
  (void)argv;
  global_view = view_new(WIDTH, HEIGHT);
  global_mesh = mesh_new();
  mesh_gen_bbox(global_mesh, b, DIM3, bv, be);
  print("%u elements\n", ment_count(global_mesh, mesh_elem(global_mesh)));
  print_quality_stats(global_mesh);
  view_focus(global_view, mesh_bbox(global_mesh));
  render();
}

void back_stop(void)
{
  mesh_free(global_mesh);
  view_free(global_view);
}

unsigned char* back_pixels(void)
{
  return (unsigned char*)(view_image(global_view)->p[0]);
}

unsigned back_width(void)
{
  return WIDTH;
}

unsigned back_height(void)
{
  return HEIGHT;
}

void back_mouse_down(double x, double y)
{
  view_click(global_view, x, y);
}

void back_mouse_up(double x, double y)
{
  view_drag(global_view, x, y);
  render();
}

void back_key_down(char k)
{
  switch (k) {
    case 'p':
      view_set_mode(global_view, VIEW_PAN);
      break;
    case 'z':
      view_set_mode(global_view, VIEW_ZOOM);
      break;
  };
  global_key = k;
}

void back_key_up(void)
{
  switch (global_key) {
    case 'p':
    case 'z':
      view_set_mode(global_view, VIEW_ROT);
      return;
    case 'r':
      mesh_refine_all(global_mesh);
      print("%u elements\n", ment_count(global_mesh, mesh_elem(global_mesh)));
      print_quality_stats(global_mesh);
      break;
  };
  render();
}
