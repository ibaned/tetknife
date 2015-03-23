#ifndef IBARRIER_H
#define IBARRIER_H

#include "my_mpi.h"

void ibarrier_begin(mpi* m);
int ibarrier_done(void);

#endif
