#include "../migrate.h"
#include "../mesh_bbox.h"
#include "../comm.h"
#include "../remotes.h"
#include "../basics.h"

static void print_elem(mesh* m, ment e, mlabel* plan)
{
  ment v[SIMPLEX_MAX_DOWN];
  unsigned nv;
  unsigned i;
  nv = ment_verts(m, e, v);
  print("%s %d {", simplex_name[e.t], e.i);
  for (i = 0; i < nv; ++i)
    print(" %d", v[i].i);
  print("}");
  if (plan)
    print(" to %ld", mlabel_get(plan, e));
  print("\n");
}

static void print_vert(mesh* m, ment v)
{
  point x;
  rent re;
  rcopy rc;
  x = mesh_point(m, v);
  print("vert %d (%f,%f,%f)", v.i, x.x, x.y, x.z);
  for (re = rent_of_f(m, v); rent_ok(re); re = rent_of_n(m, re)) {
    rc = rent_copy(m, re);
    print(" (%d,%d)", rc.rank, rc.ri);
  }
  print("\n");
}

static void print_local_mesh(mesh* m, mlabel* plan)
{
  ment e;
  for (e = ment_f(m, VERTEX); ment_ok(e); e = ment_n(m, e))
    print_vert(m, e);
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    print_elem(m, e, plan);
}

static void print_mesh(mesh* m, mlabel* plan)
{
  int i;
  for (i = 0; i < comm_size(); ++i) {
    if (i == comm_rank()) {
      print("rank %d\n", i);
      print_local_mesh(m, plan);
    }
    mpi_barrier(mpi_world());
  }
}

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
  plan = migration_plan_new(m);
  if (!comm_rank())
    mlabel_set(plan, be[0], 1);
  print_mesh(m, plan);
  migrate(m, plan);
  mlabel_free(plan);
  print_mesh(m, 0);
  mesh_free(m);
  comm_finalize();
  mpi_finalize();
  return 0;
}
