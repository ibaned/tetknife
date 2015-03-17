#include "cad_geom.h"
#include "flex.h"
#include "param.h"
#include "simplex.h"
#include "basics.h"

typedef struct {
  geom_type t;
  int i;
} ent;

struct geom {
  cad* c;
  ent* e[CAD_ENT_TYPES];
  point* pts;
  flex facs;
  parc* acs;
  flex fpls;
  pplane* pls;
  flex fcls;
  pcylinder* cls;
};

int const geom_curved[GEOM_TYPES] = {
  0,
  0,
  1,
  0,
  1,
  0
};

geom* geom_new(cad* c)
{
  geom* g;
  gent_type t;
  g = my_malloc(sizeof(*g));
  g->c = c;
  for (t = 0; t < CAD_ENT_TYPES; ++t)
    g->e[t] = 0;
  g->pts = 0;
  flex_init(&g->facs);
  g->acs = 0;
  flex_init(&g->fpls);
  g->pls = 0;
  flex_init(&g->fcls);
  g->cls = 0;
  cad_set_geom(c, g);
  return g;
}

void geom_free(geom* g)
{
  gent_type t;
  for (t = 0; t < CAD_ENT_TYPES; ++t)
    my_free(g->e[t]);
  my_free(g->pts);
  flex_dtor(&g->facs);
  my_free(g->acs);
  flex_dtor(&g->fpls);
  my_free(g->pls);
  flex_dtor(&g->fcls);
  my_free(g->cls);
  cad_set_geom(g->c, 0);
  my_free(g);
}

static ent get_ent(cad* c, gent e)
{
  ent a;
  if (e.t == CAD_VERTEX) {
    a.t = GEOM_POINT;
    a.i = e.i;
  } else {
    a = cad_geom(c)->e[e.t][e.i];
  }
  return a;
}

geom_type geom_typeof(cad* c, gent e)
{
  return get_ent(c, e).t;
}

point geom_point(cad* c, gent e)
{
  return cad_geom(c)->pts[e.i];
}

void geom_add_point(cad* c, gent e, point x)
{
  ASSERT(e.t == CAD_VERTEX);
  cad_geom(c)->pts[e.i] = x;
}

void geom_add_line(cad* c, gent e)
{
  ASSERT(e.t == CAD_EDGE);
  cad_geom(c)->e[e.t][e.i].t = GEOM_LINE;
}

void geom_add_arc(cad* c, gent e, point o, point n, point x)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a.t = GEOM_ARC;
  if (flex_full(&g->facs))
    REALLOC(g->acs, flex_grow(&g->facs));
  a.i = flex_add(&g->facs);
  g->e[e.t][e.i] = a;
  parc_init(g->acs + a.i, o, n, x);
}

typedef struct {
  cad* c;
  pplane* p;
  dim d;
  int padding_;
} by_points;

static void by_points_op(gent e, void* a)
{
  by_points* b = a;
  if (e.t == CAD_VERTEX)
    pplane_points_add(b->p, geom_point(b->c, e), &b->d);
}

static int plane_by_points(cad* c, gent e, pplane* p)
{
  by_points b;
  b.c = c;
  b.p = p;
  pplane_points_start(&b.d);
  cad_for_bnd(c, e, by_points_op, &b);
  return pplane_points_done(b.d);
}

typedef struct {
  cad* c;
  frame* f;
  int done;
  int padding_;
} by_arc;

static parc* geom_arc(cad* c, gent e)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a = g->e[e.t][e.i];
  return g->acs + a.i;
}

static void by_arc_op(gent e, void* a)
{
  by_arc* b = a;
  if (b->done)
    return;
  if (geom_typeof(b->c, e) == GEOM_ARC)
    *(b->f) = geom_arc(b->c, e)->f;
  b->done = 1;
}

static int frame_by_arc(cad* c, gent e, frame* f)
{
  by_arc b;
  b.c = c;
  b.f = f;
  b.done = 0;
  cad_for_bnd(c, e, by_arc_op, &b);
  return b.done;
}

void geom_add_plane(cad* c, gent e)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a.t = GEOM_PLANE;
  if (flex_full(&g->fpls))
    REALLOC(g->pls, flex_grow(&g->fpls));
  a.i = flex_add(&g->fpls);
  g->e[e.t][e.i] = a;
  if (plane_by_points(c, e, g->pls + a.i))
    return;
  if (frame_by_arc(c, e, &g->pls[a.i].f))
    return;
  die("couldn't auto-parameterize plane\n");
}

void geom_add_cylinder(cad* c, gent e)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a.t = GEOM_CYLINDER;
  if (flex_full(&g->fcls))
    REALLOC(g->cls, flex_grow(&g->fcls));
  a.i = flex_add(&g->fcls);
  g->e[e.t][e.i] = a;
  if (frame_by_arc(c, e, &g->cls[a.i].f))
    return;
  die("couldn't auto-parameterize cylinder\n");
}

static line geom_line(cad* c, gent e)
{
  gent v[2];
  cad_edge_verts(c, e, v);
  return line_new(geom_point(c, v[0]), geom_point(c, v[1]));
}

point geom_eval(cad* c, gent e, point uv)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a = get_ent(c, e);
  switch (a.t) {
    case GEOM_POINT:
      return geom_point(c, e);
    case GEOM_LINE:
      return line_eval(geom_line(c, e), uv.x);
    case GEOM_ARC:
      return parc_eval(g->acs + a.i, uv);
    case GEOM_PLANE:
      return pplane_eval(g->pls + a.i, uv);
    case GEOM_CYLINDER:
      return pcylinder_eval(g->cls + a.i, uv);
    case GEOM_REGION:
      return uv;
    case GEOM_TYPES:
      break;
  };
  die("bad geom_type %d in geom_eval\n", a.t);
}

point geom_uneval(cad* c, gent e, point x)
{
  geom* g;
  ent a;
  g = cad_geom(c);
  a = get_ent(c, e);
  switch (a.t) {
    case GEOM_POINT:
      return point_zero;
    case GEOM_LINE:
      return point_new(line_uneval(geom_line(c, e), x), 0, 0);
    case GEOM_ARC:
      return parc_uneval(g->acs + a.i, x);
    case GEOM_PLANE:
      return pplane_uneval(g->pls + a.i, x);
    case GEOM_CYLINDER:
      return pcylinder_uneval(g->cls + a.i, x);
    case GEOM_REGION:
      return x;
    case GEOM_TYPES:
      break;
  };
  die("bad geom_type %d in geom_uneval\n", a.t);
}

point geom_reparam(cad* c, gent from, point x, gent onto)
{
  return geom_uneval(c, onto, geom_eval(c, from, x));
}

void geom_grow_gents(geom* g, gent_type t, unsigned c)
{
  if (t == CAD_VERTEX)
    REALLOC(g->pts, c);
  else
    REALLOC(g->e[t], c);
}

