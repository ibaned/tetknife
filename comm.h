#ifndef COMM_H
#define COMM_H

#include "my_mpi.h"

void comm_init(mpi* m);
void comm_finalize(void);

int comm_rank(void);
int comm_size(void);

void comm_pack(void const* data, unsigned size, int to);
#define COMM_PACK(o,to) comm_pack(&(o),sizeof(o),to)
void comm_exch(void);
int comm_recv(void);
void comm_unpack(void* data, unsigned size);
#define COMM_UNPACK(o) comm_unpack(&(o),sizeof(o))
int comm_from(void);

#endif
