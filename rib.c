#include "rib.h"
#include "basics.h"

static point center_of_mass(unsigned n, rib_obj o[])
{
  point com;
  unsigned i;
  com = point_zero;
  for (i = 0; i < n; ++i)
    com = point_add(com, o[i].p);
  return point_scale(com, 1.0 / n);
}

/*
http://en.wikipedia.org/wiki/Moment_of_inertia#Angular_momentum
*/

static basis inertia_contrib(rib_obj* o, point com)
{
  basis a;
  a = basis_cross(point_sub(o->p, com));
  return basis_cat(a, a);
}

static basis inertia_matrix(unsigned n, rib_obj o[], point com)
{
  basis a;
  unsigned i;
  a = basis_zero;
  for (i = 0; i < n; ++i)
    a = basis_add(a, inertia_contrib(o + i, com));
  return basis_scale(a, -1.0);
}

static point min_eigenvec(basis m)
{
  unsigned nw;
  double w[3];
  unsigned best;
  unsigned i;
  nw = basis_eigenvals(m, w);
  best = 0;
  for (i = 1; i < nw; ++i)
    if (ABS(w[i]) < ABS(w[best]))
      best = i;
  return basis_eigenvec(m, w[best]);
}

static unsigned count_in(unsigned n, rib_obj o[], plane p)
{
  unsigned i;
  unsigned nin = 0;
  for (i = 0; i < n; ++i)
    if (plane_has(p, o[i].p))
      ++nin;
  return nin;
}

static plane median_plane(unsigned n, rib_obj o[], point axis)
{
  double d, min, max;
  plane p;
  unsigned i;
  unsigned nin;
  min = max = point_dot(o[0].p, axis);
  for (i = 1; i < n; ++i) {
    d = point_dot(o[i].p, axis);
    min = MIN(min, d);
    max = MAX(max, d);
  }
  d   = (max - min) / 2.0 + epsilon;
  p.r = (max + min) / 2.0;
  p.n = axis;
  for (i = 0; i < 100; ++i) {
    nin = count_in(n, o, p);
    if (nin == n / 2)
      return p;
    if (nin < n / 2)
      p.r -= d;
    else
      p.r += d;
    d /= 2.0;
  }
  die("median plane still not found after 100 bisections\n");
}

static void partition(unsigned n, rib_obj o[], plane mp)
{
  unsigned i, j;
  rib_obj so;
  j = 0;
  for (i = 0; i < n; ++i) {
    if (plane_has(mp, o[i].p)) {
      so = o[i];
      o[i] = o[j];
      o[j] = so;
      ++j;
    }
  }
  ASSERT(j == n / 2);
}

static void bisect(unsigned n, rib_obj o[])
{
  /* holy lambdas, batman ! */
  partition(n, o, median_plane(n, o, min_eigenvec(
          inertia_matrix(n, o, center_of_mass(n, o)))));
}

void rib_sort(unsigned n, rib_obj o[])
{
  unsigned a, b;
  if (n <= 1)
    return;
  a = n / 2;
  b = n - a;
  bisect(n, o);
  rib_sort(a, o);
  rib_sort(b, o + a);
}
