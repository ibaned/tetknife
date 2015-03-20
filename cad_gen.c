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

static gent last_edge(cad* c, gbnd b)
{
  return guse_of(c, guse_by_l(c, b));
}

static gent first_edge(cad* c, gbnd b)
{
  return guse_of(c, guse_by_f(c, b));
}

static int edge_has_vert(cad* c, gent e, gent v)
{
  gent ev[2];
  cad_edge_verts(c, e, ev);
  return gent_eq(ev[0], v) || gent_eq(ev[1], v);
}

static gent common_vert(cad* c, gent e0, gent e1)
{
  gent v0[2];
  unsigned i;
  cad_edge_verts(c, e0, v0);
  for (i = 0; i < 2; ++i)
    if (edge_has_vert(c, e1, v0[i]))
      return v0[i];
  return gent_null;
}

static gent other_vert(cad* c, gent e, gent v)
{
  gent ev[2];
  cad_edge_verts(c, e, ev);
  return (gent_eq(ev[0], v)) ? ev[1] : ev[0];
}

static gent common_edge(cad* c, gent v0, gent v1)
{
  guse u;
  gent e;
  for (u = guse_of_f(c, v0); guse_ok(u); u = guse_of_n(c, u)) {
    e = gbnd_of(c, guse_by(c, u));
    if (edge_has_vert(c, e, v1))
      return e;
  }
  return gent_null;
}

static gent extrude_vert(cad* c, gent v, point along)
{
  gent ov;
  gent xe;
  ov = cad_gen_point(c, point_add(geom_point(c, v), along));
  xe = cad_gen_edge(c, v, ov);
  geom_add_line(c, xe);
  return ov;
}

static gent extrude_edge(cad* c, gent e, gent pv, gent nv, gent opv, gent onv,
    gbnd sh)
{
  gent oe;
  gent xpe;
  gent xne;
  gent f;
  gbnd b;
  geom_type gt;
  oe = cad_gen_edge(c, opv, onv);
  xpe = common_edge(c, pv, opv);
  xne = common_edge(c, nv, onv);
  f = gent_new(c, CAD_FACE);
  b = gbnd_of_new(c, f);
  guse_new(c, e, b);
  guse_new(c, xne, b);
  guse_new(c, oe, b);
  guse_new(c, xpe, b);
  gt = geom_typeof(c, e);
  if (gt == GEOM_LINE) {
    geom_add_line(c, oe);
    geom_add_plane(c, f);
  } else if (gt == GEOM_ARC) {
    geom_add_arc_by_frame(c, oe, geom_arc(c, e)->f);
    geom_add_cylinder(c, f);
  }
  guse_new(c, f, sh);
  return oe;
}

static gbnd extrude_loop(cad* c, gbnd b, point along, gbnd sh)
{
  gbnd ob;
  guse u;
  gent e;
  gent oe;
  gent pv;
  gent nv;
  gent opv;
  gent onv;
  ob = gbnd_new(c, b.t);
  nv = common_vert(c, first_edge(c, b), last_edge(c, b));
  onv = extrude_vert(c, nv, along);
  for (u = guse_by_f(c, b); guse_ok(u); u = guse_by_n(c, u)) {
    pv = nv;
    opv = onv;
    e = guse_of(c, u);
    nv = other_vert(c, e, pv);
    onv = extrude_vert(c, nv, along);
    oe = extrude_edge(c, e, pv, nv, opv, onv, sh);
    guse_new(c, oe, ob);
  }
  return ob;
}

gbnd cad_extrude_face(cad* c, gent f, point along)
{
  gent of;
  gbnd b;
  gbnd ob;
  gbnd sh;
  of = gent_new(c, CAD_FACE);
  sh = gbnd_new(c, CAD_SHELL);
  for (b = gbnd_of_f(c, f); gbnd_ok(b); b = gbnd_of_n(c, b)) {
    ob = extrude_loop(c, b, along, sh);
    gbnd_set_of(c, ob, of);
  }
  geom_add_plane(c, of);
  return sh;
}
