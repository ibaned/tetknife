#include "space.h"
#include "basics.h"

/* calm down, clang. its ok. */
#pragma clang diagnostic ignored "-Wfloat-equal"

double const my_pi = 3.14159265358979323846;
double const epsilon = 1e-10;

#define POINT_ZERO {0,0,0}
point const point_zero = POINT_ZERO;
#define POINT_X {1,0,0}
#define POINT_Y {0,1,0}
#define POINT_Z {0,0,1}
point const point_x = POINT_X;
point const point_y = POINT_Y;
point const point_z = POINT_Z;
#define BASIS_IDENT {POINT_X,POINT_Y,POINT_Z}
basis const basis_ident = BASIS_IDENT;
frame const frame_ident = {BASIS_IDENT,POINT_ZERO};

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

int point_lex(point a, point b)
{
  if (a.x != b.x)
    return a.x < b.x;
  if (a.y != b.y)
    return a.y < b.y;
  if (a.z != b.z)
    return a.z < b.z;
  return 0;
}

double* point_arr(point* p)
{
  return (double*)(&p->x);
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
  return basis_new(point_new(    0,  p.z, -p.y),
                   point_new( -p.z,    0,  p.x),
                   point_new(  p.y, -p.x,    0));
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

static basis point_tprod(point a, point b)
{
  return basis_new(point_scale(a, b.x),
                   point_scale(a, b.y),
                   point_scale(a, b.z));
}

basis basis_rot(point u, double a)
{
  return basis_add(basis_scale(basis_ident, my_cos(a)),
         basis_add(basis_scale(basis_cross(u), my_sin(a)),
                   basis_scale(point_tprod(u, u), (1.0 - my_cos(a)))));
}

basis basis_add(basis a, basis b)
{
  return basis_new(point_add(a.x, b.x),
                   point_add(a.y, b.y),
                   point_add(a.z, b.z));
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

frame frame_trans(point x)
{
  return frame_new(basis_ident, x);
}

frame frame_scale(double s)
{
  return frame_new(basis_scale(basis_ident, s), point_zero);
}

bbox bbox_new(void)
{
  bbox b;
  b.min = point_new(my_dbl_max, my_dbl_max, my_dbl_max);
  b.max = point_neg(b.min);
  return b;
}

static point point_min(point a, point b)
{
  return point_new(MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z));
}

static point point_max(point a, point b)
{
  return point_new(MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z));
}

bbox bbox_add(bbox b, point p)
{
  b.min = point_min(b.min, p);
  b.max = point_max(b.max, p);
  return b;
}

