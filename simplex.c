#include "simplex.h"

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
