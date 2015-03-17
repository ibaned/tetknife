#include "cad_gen.h"
#include "cad_geom.h"

gent cad_gen_point(cad* c, point p)
{
  gent e;
  e = gent_new(c, CAD_VERTEX);
  geom_add_point(c, e, p);
  return e;
}

gent cad_gen_edge(cad* c, gent v0, gent v1)
{
  gent e;
  gbnd b;
  e = gent_new(c, CAD_EDGE);
  b = gbnd_of_new(c, e);
  guse_new(c, v0, b);
  guse_new(c, v1, b);
  return e;
}

static gent path_first_edge(cad* c, gbnd b)
{
  return guse_of(c, guse_by_f(c, b));
}

static gent path_first_vert(cad* c, gbnd b)
{
  gent v[2];
  cad_edge_verts(c, path_first_edge(c, b), v);
  return v[0];
}

static void add_path_edge(cad* c, gbnd b, gent pv, gent nv)
{
  gent e;
  e = cad_gen_edge(c, pv, nv);
  guse_new(c, e, b);
}

gbnd cad_gen_loop(cad* c, unsigned npts, point const pts[])
{
  gbnd b;
  gent pv;
  gent nv;
  unsigned i;
  b = gbnd_new(c, CAD_LOOP);
  nv = cad_gen_point(c, pts[0]);
  for (i = 1; i < npts; ++i) {
    pv = nv;
    nv = cad_gen_point(c, pts[i]);
    add_path_edge(c, b, pv, nv);
  }
  add_path_edge(c, b, nv, path_first_vert(c, b));
  return b;
}

gbnd cad_gen_line_loop(cad* c, unsigned npts, point const pts[])
{
  gbnd b;
  guse u;
  b = cad_gen_loop(c, npts, pts);
  for (u = guse_by_f(c, b); guse_ok(u); u = guse_by_n(c, u))
    geom_add_line(c, guse_of(c, u));
  return b;
}

gbnd cad_gen_circle(cad* c, point o, point n, point x)
{
  point pts[2];
  gbnd b;
  guse u;
  pts[0] = point_add(o, x);
  pts[1] = point_sub(o, x);
  b = cad_gen_loop(c, 2, pts);
  u = guse_by_f(c, b);
  geom_add_arc(c, guse_of(c, u), o, n, x);
  u = guse_by_n(c, u);
  geom_add_arc(c, guse_of(c, u), o, n, point_neg(x));
  return b;
}

gent cad_gen_plane(cad* c, gbnd b)
{
  gent e;
  e = gent_new(c, CAD_FACE);
  gbnd_set_of(c, b, e);
  geom_add_plane(c, e);
  return e;
}

gent cad_gen_polygon(cad* c, unsigned npts, point const pts[])
{
  return cad_gen_plane(c, cad_gen_line_loop(c, npts, pts));
}

gent cad_gen_disk(cad* c, point o, point n, point x)
{
  return cad_gen_plane(c, cad_gen_circle(c, o, n, x));
}
