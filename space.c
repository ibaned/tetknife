#include "space.h"
#include "basics.h"

double const my_pi = 3.14159265358979323846;
double const epsilon = 1e-10;

point const point_zero = {0,0,0};

point point_new(double x, double y, double z)
{
  point p;
  p.x = x;
  p.y = y;
  p.z = z;
  return p;
}

double point_dot(point a, point b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

double point_mag(point p)
{
  return my_sqrt(point_dot(p, p));
}

point point_add(point a, point b)
{
  return point_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

point point_sub(point a, point b)
{
  return point_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

point point_neg(point p)
{
  return point_new(-p.x, -p.y, -p.z);
}

point point_scale(point p, double s)
{
  return point_new(p.x * s, p.y * s, p.z * s);
}

point point_cross(point a, point b)
{
  return basis_eval(basis_cross(a), b);
}

point point_proj(point a, point b)
{
  /* a.b = |a||b|cos(t)
     mag(proj) = |a|cos(t)
     dir(proj) = dir(b)
     proj = (b / |b|) * (a.b / |b|)
     proj = b * a.b / b.b */
  return point_scale(b, point_dot(a, b) / point_dot(b, b));
}

point point_rej(point a, point b)
{
  return point_sub(a, point_proj(a, b));
}

point point_norm(point p)
{
  return point_scale(p, 1.0 / point_mag(p));
}

basis basis_new(point x, point y, point z)
{
  basis b;
  b.x = x;
  b.y = y;
  b.z = z;
  return b;
}

point basis_eval(basis b, point p)
{
  return point_add(point_scale(b.x, p.x),
         point_add(point_scale(b.y, p.y),
                   point_scale(b.z, p.z)));
}

double basis_det(basis b)
{
  return point_dot(b.x, point_cross(b.y, b.z));
}

basis basis_trans(basis b)
{
  return basis_new(point_new(b.x.x, b.y.x, b.z.x),
                   point_new(b.x.y, b.y.y, b.z.y),
                   point_new(b.x.z, b.y.z, b.z.z));
}

basis basis_scale(basis b, double s)
{
  return basis_new(point_scale(b.x, s),
                   point_scale(b.y, s),
                   point_scale(b.z, s));
}

basis basis_cross(point p)
{
  return basis_new(point_new(    0, -p.z,  p.y),
                   point_new(  p.z,    0, -p.x),
                   point_new( -p.y,  p.x,    0));
}

basis basis_inv(basis b)
{
  return basis_scale(basis_trans(basis_new(point_cross(b.y, b.z),
                                           point_cross(b.z, b.x),
                                           point_cross(b.x, b.y))),
                     1.0 / basis_det(b));
}

basis basis_cat(basis a, basis b)
{
  return basis_new(basis_eval(a, b.x),
                   basis_eval(a, b.y),
                   basis_eval(a, b.z));
}

frame frame_new(basis b, point o)
{
  frame f;
  f.b = b;
  f.o = o;
  return f;
}

point frame_eval(frame f, point x)
{
  return point_add(basis_eval(f.b, x), f.o);
}

frame frame_inv(frame f)
{
  frame i;
  i.b = basis_inv(f.b);
  i.o = point_scale(basis_eval(i.b, f.o), -1);
  return i;
}

point frame_uneval(frame f, point x)
{
  return frame_eval(frame_inv(f), x);
}

frame frame_cat(frame a, frame b)
{
  return frame_new(basis_cat(a.b, b.b),
                   point_add(basis_eval(a.b, b.o), a.o));
}
