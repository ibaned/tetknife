#ifndef CAD_H
#define CAD_H

typedef struct cad_struct cad;

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

int gent_eq(gent a, gent b);
int gent_ok(gent e);

int gbnd_ok(gbnd b);

int guse_ok(guse u);

cad* cad_new(void);
void cad_free(cad* c);

gent gent_new(cad* c, gent_type t);
void gent_free(cad* c, gent e);

gent gent_f(cad* c, gent_type t);
gent gent_n(cad* c, gent e);

gbnd gbnd_new(cad* c, gbnd_type t);
gbnd gbnd_of_new(cad* c, gent e);
void gbnd_free(cad* c, gbnd b);

void gbnd_of_set(cad* c, gbnd b, gent e);
gent gbnd_of(cad* c, gbnd b);

gbnd gbnd_of_f(cad* c, gent e);
gbnd gbnd_of_n(cad* c, gbnd b);

guse guse_new(cad* c, gent e, gbnd b);
void guse_free(cad* c, guse u);

gent guse_of(cad* c, guse u);
gbnd guse_by(cad* c, guse u);

guse guse_of_f(cad* c, gent e);
guse guse_of_n(cad* c, guse u);

guse guse_by_f(cad* c, gbnd b);
guse guse_by_n(cad* c, guse u);

#endif