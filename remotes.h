#ifndef REMOTES_H
#define REMOTES_H

#include "mesh.h"

typedef struct {
  int i;
} rpeer;

typedef struct {
  rpeer p;
  int i;
} rent;

typedef struct {
  int rank;
  int ri;
} rcopy;

typedef struct remotes remotes;

remotes* remotes_new(mesh* m);
void remotes_free(remotes* rs);

int rpeer_ok(rpeer p) __attribute__((const));

rpeer rpeer_f(mesh* m) __attribute__((pure));
rpeer rpeer_n(mesh* m, rpeer rp) __attribute__((pure));

rpeer rpeer_by_rank(mesh* m, int rank);
int rpeer_rank(mesh* m, rpeer rp);
void rpeer_set_rank(mesh* m, rpeer rp, int rank);

unsigned rpeer_count(mesh* m);

int rent_ok(rent re) __attribute__((const));
int rent_eq(rent a, rent b) __attribute__((const));

rent rent_f(mesh* m, rpeer rp) __attribute__((pure));
rent rent_n(mesh* m, rent re) __attribute__((pure));

unsigned rent_count(mesh* m, rpeer rp);

rent rent_of_f(mesh* m, ment e) __attribute__((pure));
rent rent_of_n(mesh* m, rent re) __attribute__((pure));
rcopy rent_copy(mesh* m, rent re) __attribute__((pure));
ment rent_of(mesh* m, rent re) __attribute__((pure));

unsigned rent_of_count(mesh* m, ment e);

rent rent_by_rank(mesh* m, ment e, int rank);
int ment_shared_with(mesh* m, ment e, int rank);
int ment_shared(mesh* m, ment e);

rent rent_new(mesh* m, ment e, rcopy rc);
void rent_free(mesh* m, rent re);

int ment_owner(mesh* m, ment e) __attribute__((pure));
rcopy ment_owner_copy(mesh* m, ment e) __attribute__((pure));
void ment_set_owner(mesh* m, ment e, int rank);
int ment_owned(mesh* m, ment e);

void rent_set_index(mesh* m, rent re, int ri);

void remotes_grow_verts(mesh* m, unsigned cap);

void remotes_add_vert(mesh* m, ment v);

void pack_remote(rcopy rc);
ment unpack_local(simplex t);

void remotes_decide_owners(mesh* m);
int remotes_less(mesh* m, int rank_a, int rank_b);

#endif
