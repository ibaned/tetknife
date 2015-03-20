#include "view.h"
#include "draw.h"
#include "basics.h"

typedef struct {
  point centroid;
  double zoom;
  basis rot;
  point place;
} globe;

static void globe_init(globe* g)
{
  g->centroid = point_zero;
  g->zoom = 1;
  g->rot = basis_ident;
  g->place = point_zero;
}

static void globe_rot(globe* g, double spin, double tilt)
{
  point about;
  double by;
  if (!tilt && !spin)
    about = point_z;
  else
    about = point_norm(point_new(-tilt, spin, 0));
  by = point_mag(point_new(spin, tilt, 0));
  g->rot = basis_cat(basis_rot(about, by), g->rot);
}

static void globe_zoom(globe* g, double by)
{
  g->zoom *= by;
}

static void globe_pan(globe* g, double x, double y)
{
  g->place.x += x;
  g->place.y += y;
}

static frame globe_frame(globe* g)
{
  frame f;
  f = frame_ident;
  /* first subtract the centroid from the points */
  f = frame_cat(frame_trans(point_scale(g->centroid, -1)), f);
  /* then zoom to the right magnification */
  f = frame_cat(frame_scale(g->zoom), f);
  /* then apply the rotations */
  f = frame_cat(frame_new(g->rot, point_zero), f);
  /* finally, shift to the on-screen center */
  f = frame_cat(frame_trans(g->place), f);
  return f;
}

struct view {
  globe g;
  frame f;
  view_mode mode;
  int padding_;
  struct { double x; double y; } pointer;
  drawing d;
};

struct view* view_new(unsigned w, unsigned h)
{
  view* v;
  v = my_malloc(sizeof(*v));
  globe_init(&v->g);
  v->f = frame_ident;
  v->mode = VIEW_ROT;
  drawing_init(&v->d, w, h);
  return v;
}

void view_free(view* v)
{
  drawing_dtor(&v->d);
  my_free(v);
}

void view_focus(view* v, bbox bb)
{
  point bbc;
  double bbr;
  point imc;
  double imr;
  bbc = point_scale(point_sub(bb.max, bb.min), 1.0/2.0);
  bbr = point_mag(bbc);
  /* avoid a div by zero for a zero-size bounding box
     (there may be less than one point used to compute
      the bounding box) */
  if (!bbr)
    bbr = 1;
  imc = point_scale(point_new(v->d.im.w, v->d.im.h, 0), 1.0/2.0);
  imr = MIN(imc.x, imc.y);
  v->g.centroid = point_add(bbc, bb.min);
  v->g.place = imc;
  v->g.zoom = imr / bbr;
  v->f = globe_frame(&v->g);
}

void view_rot(view* v, double spin, double tilt)
{
  globe_rot(&v->g, spin, tilt);
  v->f = globe_frame(&v->g);
}

void view_zoom(view* v, double by)
{
  globe_zoom(&v->g, by);
  v->f = globe_frame(&v->g);
}

void view_pan(view* v, double x, double y)
{
  globe_pan(&v->g, x, y);
  v->f = globe_frame(&v->g);
}

void view_set_mode(view* v, view_mode m)
{
  v->mode = m;
}

void view_click(view* v, double x, double y)
{
  v->pointer.x = x;
  v->pointer.y = y;
}

void view_drag(view* v, double x, double y)
{
  double dx, dy;
  dx = x - v->pointer.x;
  dy = y - v->pointer.y;
  switch (v->mode) {
    case VIEW_ROT:
      dx = (-dx) * (my_pi) / v->d.im.w; 
      dy = (-dy) * (my_pi) / v->d.im.h;
      view_rot(v, dx, dy);
      break;
    case VIEW_PAN:
      view_pan(v, dx, dy);
      break;
    case VIEW_ZOOM:
      if (dy < 0)
        dy = 1.0 - ((-dy) / (2.0 * v->d.im.h));
      else
        dy = 1.0 + (dy / v->d.im.h);
      view_zoom(v, dy);
      break;
  }
}

void view_clear(view* v, color c)
{
  drawing_clear(&v->d, c);
}

void view_point(view* v, point p, color c)
{
  draw_point(&v->d, frame_eval(v->f, p), c);
}

void view_line(view* v, line l, color c)
{
  line l2;
  l2.a = frame_eval(v->f, l.a);
  l2.b = frame_eval(v->f, l.b);
  draw_line(&v->d, l2, c);
}

static color shade(color c, double by)
{
  c.r *= by;
  c.g *= by;
  c.b *= by;
  return c;
}

void view_triangle(view* v, triangle t, color c)
{
  triangle t2;
  point n;
  color c2;
  t2.a = frame_eval(v->f, t.a);
  t2.b = frame_eval(v->f, t.b);
  t2.c = frame_eval(v->f, t.c);
  n = point_norm(triangle_norm(t2));
  c2 = shade(c, ABS(n.z));
  draw_triangle(&v->d, t2, c2);
}

void view_text(view* v, const char* s, point p, color c)
{
  draw_text(&v->d, s, frame_eval(v->f, p), c);
}

image* view_image(view* v)
{
  return &v->d.im;
}
