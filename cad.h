#ifndef CAD_H
#define CAD_H

#include "space.h"

typedef struct cad cad;

typedef enum {
  CAD_VERTEX,
  CAD_EDGE,
  CAD_FACE,
  CAD_REGION,
  CAD_ENT_TYPES
} gent_type;

typedef enum {
  CAD_PAIR,
  CAD_LOOP,
  CAD_SHELL,
  CAD_BND_TYPES
} gbnd_type;

typedef enum {
  CAD_VERTEX_USE,
  CAD_EDGE_USE,
  CAD_FACE_USE,
  CAD_USE_TYPES
} guse_type;

typedef struct {
  gent_type t;
  int i;
} gent;

typedef struct {
  gbnd_type t;
  int i;
} gbnd;

typedef struct {
  guse_type t;
  int i;
} guse;

extern gent const gent_null;
extern gbnd const gbnd_null;
extern guse const guse_null;

extern char const* const gent_name[CAD_ENT_TYPES];
extern dim const gent_dim[CAD_ENT_TYPES];
extern char const* const guse_name[CAD_USE_TYPES];
extern char const* const gbnd_name[CAD_BND_TYPES];

int gent_eq(gent a, gent b) __attribute__((const));
int gent_ok(gent e) __attribute__((const));

int gbnd_ok(gbnd b) __attribute__((const));

int guse_ok(guse u) __attribute__((const));

cad* cad_new(void);
void cad_free(cad* c);

gent gent_new(cad* c, gent_type t);
void gent_free(cad* c, gent e);

gent gent_f(cad* c, gent_type t) __attribute__((pure));
gent gent_n(cad* c, gent e) __attribute__((pure));

unsigned gent_count(cad* c, gent_type t);

gbnd gbnd_new(cad* c, gbnd_type t);
gbnd gbnd_of_new(cad* c, gent e);
void gbnd_free(cad* c, gbnd b);

void gbnd_set_of(cad* c, gbnd b, gent e);
gent gbnd_of(cad* c, gbnd b) __attribute__((pure));

gbnd gbnd_of_f(cad* c, gent e) __attribute__((pure));
gbnd gbnd_of_n(cad* c, gbnd b) __attribute__((pure));

guse guse_new(cad* c, gent e, gbnd b);
void guse_free(cad* c, guse u);

gent guse_of(cad* c, guse u) __attribute__((pure));
gbnd guse_by(cad* c, guse u) __attribute__((pure));

guse guse_of_f(cad* c, gent e) __attribute__((pure));
guse guse_of_n(cad* c, guse u) __attribute__((pure));

guse guse_by_f(cad* c, gbnd b) __attribute__((pure));
guse guse_by_l(cad* c, gbnd b) __attribute__((pure));
guse guse_by_n(cad* c, guse u) __attribute__((pure));

unsigned gent_cap(cad* c, gent_type t) __attribute__((pure));

typedef void (*cad_for_op)(gent, void*);

void cad_for_bnd(cad* c, gent e, cad_for_op f, void* a);

void cad_edge_verts(cad* c, gent e, gent v[]);

struct geom;

struct geom* cad_geom(cad* c);
void cad_set_geom(cad* c, struct geom* g);

#endif
