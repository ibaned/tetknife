#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "space.h"

typedef enum {
  VERTEX,
  EDGE,
  TRIANGLE,
  TET,
  SIMPLICES
} simplex;

enum { SIMPLEX_MAX_DOWN = 6 };

extern unsigned const simplex_ndown[SIMPLICES][SIMPLICES];
extern dim const simplex_dim[SIMPLICES];

typedef struct {
  point a;
  point b;
} line;

line line_new(point a, point b);
point line_eval(line l, double x);
point line_vec(line l);
double line_uneval(line l, point p);

typedef struct {
  point a;
  point b;
  point c;
} triangle;

typedef struct {
  point a;
  point b;
  point c;
  point d;
} tet;

#endif
