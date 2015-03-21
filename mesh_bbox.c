#include "mesh_bbox.h"
#include "classif.h"

unsigned const bbox_verts[DIMS] = {
  1,
  2,
  4,
  8
};

unsigned const bbox_simplices[DIMS] = {
  1,
  1,
  2,
  6
};

static unsigned const e10[2] = {0,1};
static unsigned const* const e1[1] = {e10};
static unsigned const e20[3] = {0,1,3};
static unsigned const e21[3] = {0,3,2};
static unsigned const* const e2[2] = {e20,e21};
static unsigned const e30[4] = {0,3,1,7};
static unsigned const e31[4] = {5,0,1,7};
static unsigned const e32[4] = {5,4,0,7};
static unsigned const e33[4] = {4,6,0,7};
static unsigned const e34[4] = {0,2,3,7};
static unsigned const e35[4] = {0,6,2,7};
static unsigned const* const e3[6] = {e30,e31,e32,e33,e34,e35};
static unsigned const* const* const etables[DIMS] = {0,e1,e2,e3};

static point bbox_point(bbox b, unsigned i)
{
  double* min;
  double* max;
  double* out;
  point p;
  dim d;
  min = point_arr(&b.min);
  max = point_arr(&b.max);
  out = point_arr(&p);
  for (d = DIM0; d < DIMS; ++d) {
    if (i & bbox_verts[d])
      out[d] = max[d];
    else
      out[d] = min[d];
  }
  return p;
}

static void gen_bbox_verts(mesh* m, bbox b, dim d, ment v[])
{
  unsigned i;
  for (i = 0; i < bbox_verts[d]; ++i) {
    v[i] = ment_new(m, VERTEX, 0);
    mesh_set_point(m, v[i], bbox_point(b, i));
  }
}

static void gen_bbox_elems(mesh* m, dim d, ment v[], ment e[])
{
  unsigned i, j;
  ment ev[SIMPLEX_MAX_DOWN];
  for (i = 0; i < bbox_simplices[d]; ++i) {
    for (j = 0; j < simplex_ndown[d][VERTEX]; ++j)
      ev[j] = v[etables[d][i][j]];
    e[i] = ment_new(m, simplex_by_dim[d], ev);
  }
}

void mesh_gen_bbox(mesh* m, bbox b, dim d,
    ment v[MAX_BBOX_VERTS], ment e[MAX_BBOX_SIMPLICES])
{
  gen_bbox_verts(m, b, d, v);
  gen_bbox_elems(m, d, v, e);
}

void classif_bbox_set(mesh* m, dim d, ment v[], ment e[], gent ge)
{
  unsigned i;
  for (i = 0; i < bbox_verts[d]; ++i) {
    classif_set(m, v[i], ge);
    classif_set_point(m, v[i], mesh_point(m, v[i]));
    mesh_set_point(m, v[i], classif_eval_point(m, v[i]));
  }
  for (i = 0; i < bbox_simplices[d]; ++i)
    classif_set(m, e[i], ge);
}

bbox mesh_bbox(mesh* m)
{
  ment v;
  bbox b;
  b = bbox_new();
  for (v = ment_f(m, VERTEX); ment_ok(v); v = ment_n(m, v))
    b = bbox_add(b, mesh_point(m, v));
  return b;
}
