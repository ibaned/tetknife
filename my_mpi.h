#ifndef MY_MPI_H
#define MY_MPI_H

typedef struct mpi mpi;
typedef struct mpi_request mpi_request;

void mpi_init(void);
void mpi_finalize(void);
mpi* mpi_world(void);
mpi* mpi_copy(mpi* m);
mpi* mpi_split(mpi* m, int subgroup, int subrank);
void mpi_free(mpi* m);
int mpi_rank(mpi* m);
int mpi_size(mpi* m);
mpi_request* mpi_isend(mpi* m, void* data, unsigned size, int to);
mpi_request* mpi_issend(mpi* m, void* data, unsigned size, int to);
int mpi_recv_any(mpi* m, void** data, unsigned* size, int* from);
mpi_request* mpi_irecv(mpi* m,
    void* data, unsigned size, int from);
int mpi_done(mpi_request* r);
void mpi_barrier(mpi* m);

#endif
