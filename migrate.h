#ifndef MIGRATE_H
#define MIGRATE_H

#include "mesh.h"
#include "label.h"

mlabel* migration_plan_new(mesh* m);
void migrate(mesh* m, mlabel* l);

#endif
