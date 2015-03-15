#ifndef SPACE_H
#define SPACE_H

typedef enum {
  DIM0 = 0,
  DIM1 = 1,
  DIM2 = 2,
  DIM3 = 3,
  DIM  = 3,
  DIMS = 4
} dim;

typedef struct {
  double x;
  double y;
  double z;
} point;

typedef struct {
  point x;
  point y;
  point z;
} basis;

typedef struct {
  basis b;
  point o;
} frame;

extern double const my_pi;
extern double const epsilon;

extern point const point_zero;

point point_new(double x, double y, double z);
double point_dot(point a, point b);
double point_mag(point p);
point point_add(point a, point b);
point point_sub(point a, point b);
point point_scale(point p, double s);
point point_cross(point a, point b);
point point_proj(point a, point b);
point point_rej(point a, point b);
point point_norm(point p);

basis basis_new(point x, point y, point z);
point basis_eval(basis b, point p);
double basis_det(basis b);
basis basis_trans(basis b);
basis basis_scale(basis b, double s);
basis basis_cross(point p);
basis basis_inv(basis b);

point frame_eval(frame f, point x);
frame frame_inv(frame f);
point frame_uneval(frame f, point x);

#endif
