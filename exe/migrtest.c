#include "../migrate.h"
#include "../mesh_bbox.h"
#include "../comm.h"
#include "../remotes.h"

int main()
{
  bbox b = {{0,0,0},{1,1,0}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  mesh* m;
  mlabel* plan;
  mpi_init();
  comm_init(mpi_world());
  m = mesh_new();
  if (!comm_rank())
    mesh_gen_bbox(m, b, DIM2, bv, be);
  else
    mesh_set_elem(m, TRIANGLE);
  remotes_new(m);
  plan = migrate_mlabel_new(m);
  if (!comm_rank())
    mlabel_set(plan, be[0], 1);
  migrate(m, plan);
  mlabel_free(plan);
  mesh_free(m);
  comm_finalize();
  mpi_finalize();
  return 0;
}
