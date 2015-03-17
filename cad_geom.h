#ifndef CAD_GEOM_H
#define CAD_GEOM_H

#include "cad.h"
#include "space.h"

typedef struct geom geom;

typedef enum {
  GEOM_POINT,
  GEOM_LINE,
  GEOM_ARC,
  GEOM_PLANE,
  GEOM_CYLINDER,
  GEOM_REGION,
  GEOM_TYPES
} geom_type;

extern int const geom_curved[GEOM_TYPES];

geom* geom_new(cad* c);
void geom_free(geom* g);

geom_type geom_typeof(cad* c, gent e);
point geom_point(cad* c, gent e);
void geom_add_point(cad* c, gent e, point x);
void geom_add_line(cad* c, gent e);
void geom_add_arc(cad* c, gent e, point o, point n, point x);
void geom_add_plane(cad* c, gent e);
void geom_add_cylinder(cad* c, gent e);
point geom_eval(cad* c, gent e, point uv);
point geom_uneval_hint(cad* c, gent e, point x, point uv_hint);
point geom_uneval(cad* c, gent e, point x);
point geom_reparam(cad* c, gent from, point x, gent onto);

void geom_grow_gents(geom* g, gent_type t, unsigned c);

#endif
