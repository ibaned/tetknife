#include "simplex.h"

char const* const simplex_name[SIMPLICES] = {
  "vertex",
  "edge",
  "triangle",
  "tet"
};

unsigned const simplex_ndown[SIMPLICES][SIMPLICES] = {
  {1,0,0,0},
  {2,1,0,0},
  {3,3,1,0},
  {4,6,4,1}
};

dim const simplex_dim[SIMPLICES] = {
  DIM0,
  DIM1,
  DIM2,
  DIM3
};

simplex const simplex_by_dim[DIMS] = {
  VERTEX,
  EDGE,
  TRIANGLE,
  TET
};

line line_new(point a, point b)
{
  line l;
  l.a = a;
  l.b = b;
  return l;
}

point line_eval(line l, double x)
{
  return point_add(point_scale(l.b, x), point_scale(l.a, 1.0 - x));
}

point line_vec(line l)
{
  return point_sub(l.b, l.a);
}

double line_uneval(line l, point p)
{
  point ba;
  point pa;
  /* p = a + h(b - a)
     (p - a) = h(b - a)
     (p - a).(b - a) = h(b - a).(b - a)
     (p - a).(b - a)
     --------------- = h
     (b - a).(b - a)      */
  ba = line_vec(l);
  pa = point_sub(p, l.a);
  return point_dot(pa, ba) / point_dot(ba, ba);
}

double line_len(line l)
{
  return point_mag(line_vec(l));
}

static point triangle_cross(triangle t)
{
  return point_cross(point_sub(t.b, t.a), point_sub(t.c, t.a));
}

double triangle_area(triangle t)
{
  return point_mag(triangle_cross(t)) / 2.0;
}

point triangle_norm(triangle t)
{
  return point_norm(triangle_cross(t));
}

static basis tet_basis(tet t)
{
  return basis_new(point_sub(t.b, t.a),
                   point_sub(t.c, t.a),
                   point_sub(t.d, t.a));
}

double tet_volume(tet t)
{
  return basis_det(tet_basis(t)) / 6.0;
}

point line_centroid(line l)
{
  return point_scale(point_add(l.a, l.b),
                     1.0 / 2.0);
}

point triangle_centroid(triangle t)
{
  return point_scale(point_add(t.a,
                     point_add(t.b,
                               t.c)),
                     1.0 / 3.0);
}

point tet_centroid(tet t)
{
  return point_scale(point_add(t.a,
                     point_add(t.b,
                     point_add(t.c,
                               t.d))),
                     1.0 / 4.0);
}
