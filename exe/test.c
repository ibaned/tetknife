#include "../back.h"
#include "../view_mesh.h"
#include "../mesh_bbox.h"

#define WIDTH 640
#define HEIGHT 480
static view* global_view;
static mesh* global_mesh;

static void render(void)
{
  view_mesh_wireframe(global_view, global_mesh);
}

void back_start(void)
{
  bbox b = {{0,0,0},{1,1,1}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  global_view = view_new(WIDTH, HEIGHT);
  global_mesh = mesh_new();
  mesh_gen_bbox(global_mesh, b, DIM3, bv, be);
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
  (void)x;
  (void)y;
}

void back_mouse_up(double x, double y)
{
  (void)x;
  (void)y;
}

void back_key_down(char k)
{
  (void)k;
}

void back_key_up(void)
{
}
