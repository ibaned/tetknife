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

int ment_eq(ment a, ment b) __attribute__((const));
int ment_ok(ment e) __attribute__((const));

int muse_eq(muse a, muse b) __attribute__((const));
int muse_ok(muse e) __attribute__((const));

mesh* mesh_new(void);
void mesh_free(mesh* m);

ment ment_new(mesh* m, simplex t, ment const v[]);
void ment_free(mesh* m, ment e);

simplex mesh_element(mesh* m);

ment ment_f(mesh* m, simplex t) __attribute__((pure));
ment ment_n(mesh* m, ment e) __attribute__((pure));

unsigned ment_count(mesh* m, simplex t) __attribute__((pure));

unsigned ment_verts(mesh* m, ment e, ment v[]);

muse muse_f(mesh* m, ment v, simplex t) __attribute__((pure));
muse muse_n(mesh* m, muse u) __attribute__((pure));

ment muse_of(muse u) __attribute__((pure));

point mesh_point(mesh* m, ment v) __attribute__((pure));
void mesh_set_point(mesh* m, ment v, point x);

unsigned mesh_cap(mesh* m, simplex t) __attribute__((pure));

struct fields;
struct fields* mesh_fields(mesh* m);

struct classif;
struct classif* mesh_classif(mesh* m);
void mesh_set_classif(mesh* m, struct classif* cl);

struct mflag;
struct mflag* mesh_flag(mesh* m);
void mesh_set_flag(mesh* m, struct mflag* f);

void mesh_merge_verts(mesh* m, ment v, ment into);

#endif
