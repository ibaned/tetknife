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

typedef struct {
  point min;
  point max;
} bbox;

typedef struct {
  point n;
  double r;
} plane;

extern double const my_pi;
extern double const epsilon;

extern point const point_zero;
extern point const point_x;
extern point const point_y;
extern point const point_z;
extern basis const basis_ident;
extern basis const basis_zero;
extern frame const frame_ident;

point point_new(double x, double y, double z);
double point_dot(point a, point b);
double point_mag(point p);
point point_add(point a, point b);
point point_sub(point a, point b);
point point_neg(point p);
point point_scale(point p, double s);
point point_cross(point a, point b);
point point_proj(point a, point b);
point point_rej(point a, point b);
point point_norm(point p);
double* point_arr(point* p);
point point_perp(point p);

basis basis_new(point x, point y, point z);
point basis_eval(basis b, point p);
double basis_det(basis b);
basis basis_trans(basis b);
basis basis_scale(basis b, double s);
basis basis_cross(point p);
basis basis_inv(basis b);
basis basis_cat(basis a, basis b);
basis basis_rot(point u, double a);
basis basis_add(basis a, basis b);
basis basis_sub(basis a, basis b);
point* basis_arr(basis* b);

unsigned basis_eigenvals(basis b, double w[]);
point basis_eigenvec(basis m, double w);

frame frame_new(basis b, point c);
point frame_eval(frame f, point x);
frame frame_inv(frame f);
point frame_uneval(frame f, point x);
frame frame_cat(frame a, frame b);
frame frame_trans(point x);
frame frame_scale(double s);

bbox bbox_new(void);
bbox bbox_add(bbox b, point p);

int plane_has(plane p, point x);

#endif
