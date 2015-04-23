#ifndef CAVITY_OP_H
#define CAVITY_OP_H

#include "mesh.h"

typedef struct cavity_env cavity_env;

int cavity_check(cavity_env* env, ment e);
int cavity_check_verts(cavity_env* env, unsigned nv, ment const v[]);

typedef void (*cavity_op)(ment e, cavity_env* env);

void cavity_exec(mesh* m, cavity_op op, simplex t);

#endif
