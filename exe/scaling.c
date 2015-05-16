#include "../mesh_bbox.h"
#include "../mesh_adapt.h"
#include "../mesh_geom.h"
#include "../basics.h"
#include "../subcomm.h"
#include "../remotes.h"
#include "../rib.h"
#include "../cavity_op.h"
#include <spi/include/kernel/memory.h>

static void print_value_stats(char const* name, unsigned ln)
{
  unsigned long tn;
  unsigned maxn;
  unsigned minn;
  double avg;
  tn = mpi_add_ulong(comm_mpi(), ln);
  maxn = mpi_max_unsigned(comm_mpi(), ln);
  minn = mpi_min_unsigned(comm_mpi(), ln);
  avg = ((double) tn) / ((double) (comm_size()));
  if (!comm_rank())
    print("%s max %u min %u total %lu avg %f imb %f\n",
        name, maxn, minn, tn, avg, ((double) maxn) / avg);
}

static void print_mesh_stats(mesh* m)
{
  print_value_stats("nverts", ment_count(m, VERTEX));
  print_value_stats("nelem", ment_count(m, TET));
  print_value_stats("nneigh", rpeer_count(m));
}

static void print_memory_stats(void)
{
  uint64_t heap, heapmax, hmmap;
  Kernel_GetMemorySize(KERNEL_MEMSIZE_HEAP, &heap);
  Kernel_GetMemorySize(KERNEL_MEMSIZE_HEAPMAX, &heapmax);
  Kernel_GetMemorySize(KERNEL_MEMSIZE_MMAP, &hmmap);
  print_value_stats("heap", heap);
  print_value_stats("heapmax", heapmax);
  print_value_stats("mmap", hmmap);
}

static void refine_subgroup(mesh* m, int ngroups)
{
  mpi* oldcomm;
  int mygroup;
  double t0, t1;
  t0 = mpi_time();
  mygroup = comm_rank() % ngroups;
  oldcomm = mesh_enter_groups(
      m, mygroup,
      comm_rank() / ngroups);
  if (!mygroup) {
    mesh_refine_all(m);
    if (!comm_rank())
      print("refine, comm_size %d\n", comm_size());
    print_mesh_stats(m);
    print_memory_stats();
  }
  mesh_exit_groups(m, oldcomm);
  mpi_barrier(comm_mpi());
  t1 = mpi_time();
  if (!comm_rank())
    print("refine_subgroup took %f seconds\n", t1 - t0);
}

static void balance_subgroup(mesh* m, int ngroups)
{
  mpi* oldcomm;
  int mygroup;
  double t0, t1;
  t0 = mpi_time();
  mygroup = comm_rank() % ngroups;
  oldcomm = mesh_enter_groups(
      m, mygroup,
      comm_rank() / ngroups);
  if (!mygroup) {
    mesh_balance_rib(m);
    if (!comm_rank())
      print("balance, comm_size %d\n", comm_size());
    print_mesh_stats(m);
    print_memory_stats();
  }
  mesh_exit_groups(m, oldcomm);
  mpi_barrier(comm_mpi());
  t1 = mpi_time();
  if (!comm_rank())
    print("balance_subgroup took %f seconds\n", t1 - t0);
}

int main()
{
  int ngroups;
  mesh* m;
  bbox b = {{0,0,0},{1.2,1.1,1}};
  ment bv[MAX_BBOX_VERTS];
  ment be[MAX_BBOX_SIMPLICES];
  unsigned i;
  static unsigned const initial_refines = 14;
  double t0, ts;
  mpi_init();
  t0 = mpi_time();
  comm_init(mpi_world());
  ngroups = comm_size();
  print_memory_stats();
  m = mesh_new();
  if (!comm_rank())
    mesh_gen_bbox(m, b, DIM3, bv, be);
  else
    mesh_set_elem(m, TET);
  remotes_new(m);
  for (i = 0; i < initial_refines; ++i) {
    refine_subgroup(m, ngroups);
  }
  while (ngroups > 1) {
    ASSERT(ngroups % 2 == 0);
    refine_subgroup(m, ngroups);
    ngroups /= 2;
    balance_subgroup(m, ngroups);
    ts = mpi_time();
    if (!comm_rank())
      print("wall time: %f seconds\n", ts - t0);
  }
  mesh_free(m);
  comm_finalize();
  mpi_finalize();
  return 0;
}
