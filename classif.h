#ifndef CLASSIF_H
#define CLASSIF_H

#include "mesh.h"
#include "cad.h"

typedef struct classif classif;

classif* classif_new(mesh* m, cad* c);
void classif_free(classif* cl);

mesh* classif_mesh(classif* cl);
cad* classif_cad(classif* cl);
cad* mesh_cad(mesh* m);

gent classif_get(mesh* m, ment me);
void classif_set(mesh* m, ment me, gent ge);

void classif_rm(mesh* m, ment me);

point classif_point(mesh* m, ment me);
void classif_set_point(mesh* m, ment me, point x);

ment classif_f(mesh* m, gent ge, simplex t);
ment classif_n(mesh* m, ment me);

void classif_grow_ments(classif* cl, simplex t, unsigned c);

point classif_eval_point(mesh* m, ment me);

void classif_transfer_rebuild(mesh* m, ment oe, ment ne);

#endif
