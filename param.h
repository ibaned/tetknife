#ifndef PARAM_H
#define PARAM_H

#include "space.h"

typedef struct {
  frame f;
} pplane;

typedef struct {
  frame f;
} parc;

typedef struct {
  frame f;
} pcylinder;

void pplane_points_start(dim* d);
void pplane_points_add(pplane* p, point x, dim* d);
int pplane_points_done(dim d);
point pplane_eval(pplane* p, point uv);
point pplane_uneval(pplane* p, point x);

void parc_init(parc* a, point c, point n, point x);
point parc_eval(parc* a, point uv);
point parc_uneval(parc* a, point x);

void pcylinder_from_parc(pcylinder* c, parc* a, point n);
point pcylinder_eval(pcylinder* c, point uv);
point pcylinder_uneval(pcylinder* c, point x);

#endif
