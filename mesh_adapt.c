#include "mesh_adapt.h"
#include "mesh_adj.h"
#include "mesh_mod.h"

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
    if (q < mq) {
      mq = q;
      v[0] = v_[0];
      v[1] = v_[1];
    }
    split_cancel(s);
  }
}

static void split_ment(mesh* m, split* s, ment e)
{
  ment v[2];
  find_best_edge_split(m, s, e, v);
  split_edge(s, v);
}

void mesh_refine(mesh* m, mflag* f)
{
  ment e;
  split* s;
  s = split_new(m);
  for (e = ment_f(m, mesh_element(m)); ment_ok(e); e = ment_n(m, e))
    if (mflag_get(f, e))
      split_ment(m, s, e);
  split_free(s);
}
