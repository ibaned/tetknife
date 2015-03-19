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

static gent path_last_edge(cad* c, gbnd b)
{
  return guse_of(c, guse_by_l(c, b));
}

static gent common_vert(cad* c, gent e0, gent e1)
{
  gent v[2][2];
  unsigned i,j;
  cad_edge_verts(c, e0, v[0]);
  cad_edge_verts(c, e1, v[1]);
  for (i = 0; i < 2; ++i)
  for (j = 0; j < 2; ++j)
    if (gent_eq(v[0][i], v[1][j]))
      return v[0][i];
  return gent_null;
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
  gent fv;
  gent pv;
  gent nv;
  unsigned i;
  b = gbnd_new(c, CAD_LOOP);
  fv = nv = cad_gen_point(c, pts[0]);
  for (i = 1; i < npts; ++i) {
    pv = nv;
    nv = cad_gen_point(c, pts[i]);
    add_path_edge(c, b, pv, nv);
  }
  add_path_edge(c, b, nv, fv);
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

static gent copy_vert(cad* c, gent v)
{
  return cad_gen_point(c, geom_point(c, v));
}

static gent other_vert(cad* c, gent e, gent v)
{
  gent vs[2];
  cad_edge_verts(c, e, vs);
  if (gent_eq(vs[0], v))
    return vs[1];
  return vs[0];
}

static gent copy_edge(cad* c, gent e, gent v0, gent v1)
{
  gent oe;
  geom_type gt;
  oe = cad_gen_edge(c, v0, v1);
  gt = geom_typeof(c, e);
  if (gt == GEOM_LINE)
    geom_add_line(c, oe);
  else if (gt == GEOM_ARC)
    geom_add_arc_by_frame(c, oe, geom_arc(c, e)->f);
  return oe;
}

static gbnd copy_loop(cad* c, gbnd b)
{
  gbnd ob;
  gent pv;
  gent nv;
  gent ofv;
  gent opv;
  gent onv;
  gent e;
  gent oe;
  guse u;
  ob = gbnd_new(c, CAD_LOOP);
  nv = common_vert(c, path_first_edge(c, b), path_last_edge(c, b));
  ofv = onv = copy_vert(c, nv);
  for (u = guse_by_f(c, b); guse_ok(u); u = guse_by_n(c, u)) {
    pv = nv;
    opv = onv;
    e = guse_of(c, u);
    nv = other_vert(c, e, pv);
    onv = copy_vert(c, nv);
    oe = copy_edge(c, e, opv, onv);
    guse_new(c, oe, ob);
  }
  oe = copy_edge(c, e, onv, ofv);
  guse_new(c, oe, ob);
  return ob;
}

void cad_extrude_face(cad* c, gent f, point n)
{
  gent of;
  gbnd b;
  gbnd ob;
  of = gent_new(c, CAD_FACE);
  for (b = gbnd_of_f(c, f); gbnd_ok(b); b = gbnd_of_n(c, b)) {
    ob = copy_loop(c, b);
    (void)n;
  /*transform_loop(c, ob, frame_trans(n));
    connect_loops(c, b, ob);
    extrude_geom(c, b, ob);*/
    gbnd_set_of(c, ob, of);
  }
}
