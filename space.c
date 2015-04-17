#include "space.h"
#include "basics.h"

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
basis const basis_zero = {POINT_ZERO,POINT_ZERO,POINT_ZERO};
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

double* point_arr(point* p)
{
  return (double*)(&p->x);
}

point point_perp(point p)
{
  double* a;
  double* b;
  unsigned tmp;
  unsigned min = 0;
  unsigned mid = 1;
  unsigned max = 2;
  point q;
  a = point_arr(&p);
  b = point_arr(&q);
  /* custom bubble sort for three values */
  if (ABS(a[max]) < ABS(a[min])) {
    tmp = max;
    max = min;
    min = tmp;
  }
  if (ABS(a[mid]) < ABS(a[min])) {
    tmp = mid;
    mid = min;
    min = tmp;
  }
  if (ABS(a[max]) < ABS(a[mid])) {
    tmp = max;
    max = mid;
    mid = tmp;
  }
  ASSERT(ABS(a[min]) <= ABS(a[mid]));
  ASSERT(ABS(a[mid]) <= ABS(a[max]));
  b[mid] = -a[max];
  b[max] = a[mid];
  b[min] = 0;
  return point_norm(q);
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

basis basis_sub(basis a, basis b)
{
  return basis_new(point_sub(a.x, b.x),
                   point_sub(a.y, b.y),
                   point_sub(a.z, b.z));
}

static double basis_trace(basis b)
{
  return b.x.x + b.y.y + b.z.z;
}

unsigned basis_eigenvals(basis m, double w[])
{
  double a, b, c, d;
  a = -1;
  b = basis_trace(m);
  c = - (b * b - basis_trace(basis_cat(m, m))) / 2.0;
  d = basis_det(m);
  return cubic_roots(a, b, c, d, w);
}

point* basis_arr(basis* b)
{
  return (point*) (&b->x);
}

point basis_eigenvec(basis m, double w)
{
  basis a;
  point c[3];
  unsigned best;
  unsigned i;
  point* aa;
  a = basis_sub(m, basis_scale(basis_ident, w));
  /* we have to come up with a vector in the null
     space of (a), so the first try is to assume
     (a) has rank 2 and find the biggest cross product
     between its columns, giving the null space
     vector */
  c[0] = point_cross(a.x, a.y);
  c[1] = point_cross(a.y, a.z);
  c[2] = point_cross(a.x, a.z);
  best = 0;
  for (i = 1; i < 3; ++i)
    if (point_mag(c[i]) > point_mag(c[best]))
      best = i;
  if (point_mag(c[best]) > epsilon)
    return point_norm(c[best]);
  /* okay, rank is less than 2; all basis vectors
     are aligned. pick the biggest one and
     get a vector orthogonal to that one */
  best = 0;
  aa = basis_arr(&a);
  for (i = 1; i < 3; ++i)
    if (point_mag(aa[i]) > point_mag(aa[best]))
      best = i;
  if (point_mag(aa[best]) > epsilon)
    return point_perp(aa[best]);
  /* welp, thats an all-zero matrix. toss a coin, folks. */
  return point_x;
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

int plane_has(plane p, point x)
{
  return point_dot(p.n, x) >= p.r;
}
