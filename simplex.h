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

extern char const* const simplex_name[SIMPLICES];
extern unsigned const simplex_ndown[SIMPLICES][SIMPLICES];
extern dim const simplex_dim[SIMPLICES];
extern simplex const simplex_by_dim[DIMS];

typedef struct {
  point a;
  point b;
} line;

line line_new(point a, point b);
point line_eval(line l, double x);
point line_vec(line l);
double line_uneval(line l, point p);
double line_len(line l);

typedef struct {
  point a;
  point b;
  point c;
} triangle;

double triangle_area(triangle t);
point triangle_norm(triangle t);

typedef struct {
  point a;
  point b;
  point c;
  point d;
} tet;

double tet_volume(tet t);

point line_centroid(line l);
point triangle_centroid(triangle t);
point tet_centroid(tet t);

#endif
