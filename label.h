#ifndef LABEL_H
#define LABEL_H

#include "mesh.h"

typedef struct mlabel mlabel;

mlabel* mlabel_new(mesh* m);
void mlabel_free(mlabel* l);

long mlabel_get(mlabel* l, ment e) __attribute__((pure));
void mlabel_set(mlabel* l, ment e, long v);

#endif
