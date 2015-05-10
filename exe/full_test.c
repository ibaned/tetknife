#include "../back.h"
#include "../view_mesh.h"
#include "../mesh_bbox.h"
#include "../mesh_adapt.h"
#include "../mesh_geom.h"
#include "../basics.h"
#include "../subcomm.h"
#include "../remotes.h"
#include "../rib.h"
#include "../cavity_op.h"

#define WIDTH 640
#define HEIGHT 480
static view* global_view;
static mesh* global_mesh;
static char global_key;
static int global_groups;

static void render(void)
{
  view_mesh_parts(global_view, global_mesh);
}

void back_start(int argc, char** argv)
{
  bbox b = {{0,0,0},{1.2,1.1,1}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  (void)argc;
  (void)argv;
  global_groups = comm_size();
  global_view = view_new(WIDTH, HEIGHT);
  global_mesh = mesh_new();
  if (!comm_rank())
    mesh_gen_bbox(global_mesh, b, DIM3, bv, be);
  else
    mesh_set_elem(global_mesh, TET);
  remotes_new(global_mesh);
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

static void refine_subgroup(void)
{
  mpi* oldcomm;
  int mygroup;
  mygroup = comm_rank() % global_groups;
  oldcomm = mesh_enter_groups(
      global_mesh, mygroup,
      comm_rank() / global_groups);
  if (!mygroup)
    mesh_refine_all(global_mesh);
  mesh_exit_groups(global_mesh, oldcomm);
  mpi_barrier(comm_mpi());
}

static void balance_subgroup(void)
{
  mpi* oldcomm;
  int mygroup;
  mygroup = comm_rank() % global_groups;
  oldcomm = mesh_enter_groups(
      global_mesh, mygroup,
      comm_rank() / global_groups);
  if (!mygroup)
    mesh_balance_rib(global_mesh);
  mesh_exit_groups(global_mesh, oldcomm);
  mpi_barrier(comm_mpi());
}

void back_key_up(void)
{
  switch (global_key) {
    case 'p':
    case 'z':
      view_set_mode(global_view, VIEW_ROT);
      return;
    case 'r':
      refine_subgroup();
      break;
    case 'b':
      balance_subgroup();
      break;
    case 's':
      global_groups /= 2;
      if (!comm_rank())
        debug("global_groups = %d\n", global_groups);
      return;
  };
  render();
}
