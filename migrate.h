#ifndef MIGRATE_H
#define MIGRATE_H

#include "mesh.h"
#include "label.h"

mlabel* migrate_mlabel_new(mesh* m);
void migrate(mesh* m, mlabel* l);

#endif
