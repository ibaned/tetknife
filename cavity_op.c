#include "cavity_op.h"
#include "mesh_adj.h"
#include "remotes.h"
#include "migrate.h"
#include "basics.h"
#include "comm.h"

struct cavity_env {
  mesh* m;
  mset vs;
  int is_requesting;
  int padding_;
};

int cavity_check(cavity_env* env, ment e)
{
  unsigned nv;
  ment v[SIMPLEX_MAX_DOWN];
  nv = ment_verts(env->m, e, v);
  return cavity_check_verts(env, nv, v);
}

int cavity_check_verts(cavity_env* env, unsigned nv, ment const v[])
{
  int are_local;
  unsigned i;
  are_local = 1;
  for (i = 0; i < nv; ++i)
    if (ment_shared(env->m, v[i]))
      are_local = 0;
  if (!are_local && env->is_requesting) {
    for (i = 0; i < nv; ++i)
      mset_add(&env->vs, v[i]);
  }
  return are_local;
}

static int exch_ents(cavity_env* env)
{
  unsigned i;
  mesh* m;
  ment v;
  rent re;
  muse u;
  ment e;
  mlabel* plan;
  int ra, rb;
  m = env->m;
  if (mpi_max_unsigned(comm_mpi(), env->vs.s.n) == 0)
    return 0;
  for (i = 0; i < env->vs.s.n; ++i) {
    v = env->vs.e[i];
    for (re = rent_of_f(m, v); rent_ok(re); re = rent_of_n(m, re))
      pack_remote(rent_copy(m, re));
    /* request from self as well */
    COMM_PACK(v.i, comm_rank());
  }
  comm_exch();
  /* don't use migration_plan_new, we actually
     want uninit to be -1 so that comm_rank() means
     specifically requested by self */
  plan = mlabel_new(m);
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    mlabel_set(plan, e, -1);
  while (comm_recv()) {
    v = unpack_local(VERTEX);
    for (u = muse_f(m, v, mesh_elem(m)); muse_ok(u); u = muse_n(m, u)) {
      e = muse_of(u);
      ra = (int) mlabel_get(plan, e);
      rb = comm_from();
      if (ra == -1)
        mlabel_set(plan, e, rb);
      else {
        if (remotes_less(m, rb, ra))
          mlabel_set(plan, e, rb);
        else
          mlabel_set(plan, e, ra);
      }
    }
  }
  /* now convert back, all remaining -1's stay here */
  for (e = ment_f(m, mesh_elem(m)); ment_ok(e); e = ment_n(m, e))
    if (mlabel_get(plan, e) == -1)
      mlabel_set(plan, e, comm_rank());
  migrate(m, plan);
  return 1;
}

void cavity_exec(mesh* m, cavity_op op, simplex t)
{
  cavity_env env;
  ment e;
  env.m = m;
  mset_init(&env.vs);
  do {
    env.is_requesting = 0;
    for (e = ment_f(m, t); ment_ok(e); e = ment_n(m, e))
      op(e, &env);
    env.is_requesting = 1;
    for (e = ment_f(m, t); ment_ok(e); e = ment_n(m, e))
      op(e, &env);
  } while (exch_ents(&env));
  mset_dtor(&env.vs);
}
