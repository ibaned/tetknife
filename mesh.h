#ifndef MESH_H
#define MESH_H

#include "simplex.h"
#include "space.h"

typedef struct {
  simplex t;
  int i;
} ment;

typedef struct {
  simplex t;
  int i;
} muse;

typedef struct mesh mesh;

extern ment const ment_null;
extern muse const muse_null;

int ment_eq(ment a, ment b);
int ment_ok(ment e);

int muse_eq(muse a, muse b);
int muse_ok(muse e);

mesh* mesh_new(void);
void mesh_free(mesh* m);

ment ment_new(mesh* m, simplex t, ment const v[]);
void ment_free(mesh* m, ment e);

ment ment_f(mesh* m, simplex t);
ment ment_n(mesh* m, ment e);

unsigned ment_count(mesh* m, simplex t);

unsigned ment_verts(mesh* m, ment e, ment v[]);

muse muse_f(mesh* m, ment v, simplex t);
muse muse_n(mesh* m, muse u);

ment muse_of(muse u);

point mesh_point(mesh* m, ment v);
void mesh_set_point(mesh* m, ment v, point x);

unsigned mesh_cap(mesh* m, simplex t);

struct fields;
struct fields* mesh_fields(mesh* m);

struct classif;
struct classif* mesh_classif(mesh* m);
void mesh_set_classif(mesh* m, struct classif* cl);

#endif
