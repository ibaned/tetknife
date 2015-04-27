#include "mesh_adapt.h"
#include "mesh_adj.h"
#include "mesh_mod.h"
#include "cavity_op.h"

static void find_best_edge_split(mesh* m, split* s, ment e, ment v[2])
{
  double mq;
  double q;
  unsigned ne;
  unsigned i;
  ment v_[2];
  ne = simplex_ndown[e.t][EDGE];
  mq = -1;
  for (i = 0; i < ne; ++i) {
    mesh_down(m, e, EDGE, i, v_);
    split_start(s, EDGE, v_, ment_null);
    q = split_quality(s);
    if (q > mq) {
      mq = q;
      v[0] = v_[0];
      v[1] = v_[1];
    }
    split_cancel(s);
  }
}

static split* the_split;

static void refine_op(mesh* m, ment e)
{
  ment v[2];
  find_best_edge_split(m, the_split, e, v);
  split_edge(the_split, v);
}

void mesh_refine(mesh* m, mflag* f)
{
  the_split = split_new(m);
  cavity_exec_flagged(m, f, refine_op, mesh_elem(m));
  split_free(the_split);
}

void mesh_refine_all(mesh* m)
{
  mflag* f = mflag_new_all(m, mesh_elem(m));
  mesh_refine(m, f);
  mflag_free(f);
}
