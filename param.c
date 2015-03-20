#include "param.h"
#include "basics.h"

void pplane_points_start(dim* d)
{
  *d = DIM0;
}

void pplane_points_add(pplane* p, point x, dim* d)
{
  switch (*d) {
    case DIM0:
      p->f.o = x;
      *d = DIM1;
      return;
    case DIM1:
      p->f.b.x = point_sub(x, p->f.o);
      if (point_mag(p->f.b.x) > epsilon) {
        p->f.b.x = point_norm(p->f.b.x);
        *d = DIM2;
      }
      return;
    case DIM2:
      p->f.b.y = point_sub(x, p->f.o);
      p->f.b.y = point_rej(p->f.b.y, p->f.b.x);
      if (point_mag(p->f.b.y) > epsilon) {
        p->f.b.y = point_norm(p->f.b.y);
        p->f.b.z = point_cross(p->f.b.x, p->f.b.y);
        if (point_mag(p->f.b.z) > epsilon)
          *d = DIM3;
      }
      return;
    case DIM3:
      return;
    case DIMS:
      die("pplane_points_add\n");
  };
}

int pplane_points_done(dim d)
{
  return d == DIM3;
}

point pplane_eval(pplane* p, point uv)
{
  return frame_eval(p->f, uv);
}

point pplane_uneval(pplane* p, point x)
{
  return frame_uneval(p->f, x);
}

void parc_init(parc* a, point c, point n, point x)
{
  a->f.o = c;
  a->f.b.x = x;
  a->f.b.z = n;
  a->f.b.y = point_cross(a->f.b.z, a->f.b.x);
}

point parc_eval(parc* a, point uv)
{
  return frame_eval(a->f, point_new(my_cos(uv.x), my_sin(uv.x), 0));
}

point parc_uneval(parc* a, point x)
{
  point c;
  double t;
  c = frame_uneval(a->f, x);
  t = my_atan2(c.y, c.x);
  if (t < (my_pi / 2.0))
    t += 2.0 * my_pi;
  return point_new(t, 0, 0);
}

void pcylinder_from_parc(pcylinder* c, parc* a, point n)
{
  c->f = a->f;
  c->f.b.z = n;
}

point pcylinder_eval(pcylinder* c, point uv)
{
  return frame_eval(c->f, point_new(my_cos(uv.x), my_sin(uv.x), uv.y));
}

point pcylinder_uneval(pcylinder* pc, point x)
{
  point c;
  double t;
  c = frame_uneval(pc->f, x);
  t = my_atan2(c.y, c.x);
  if (t < (my_pi / 2.0))
    t += 2.0 * my_pi;
  return point_new(t, c.z, 0);
}
