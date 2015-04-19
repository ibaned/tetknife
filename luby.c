#include "luby.h"
#include "comm.h"
#include "mersenne_twister.h"
#include "basics.h"
#include "remotes.h"

/* Luby, Michael.
   "A simple parallel algorithm for the maximal independent set problem."
   SIAM journal on computing 15.4 (1986): 1036-1053. */

int luby_mis(unsigned nneigh, int const neigh[], int in_Vp)
{
  unsigned pi;
  unsigned neighpi;
  unsigned i;
  int in_original_Vp;
  int neigh_in_Vp;
  int in_I;
  int neigh_in_I;
  int any_neigh_in_I;
  int in_Ip;
  int is_min;
  int neigh_in_Ip;
  int in_Y;
  /* this is the weakest part of our implementation:
     the seed process. luckily, it is more a theoretical
     weakness than a practical one */
  mersenne_twister_seed((unsigned)(comm_rank() + 1));
  in_I = 0; /* I = empty */
  in_original_Vp = in_Vp;
  /* while V' != empty */
  while (mpi_max_int(comm_mpi(), in_Vp)) {
    /* also, for very large problems,
       2^32 might be less than |V|^4 */
    pi = mersenne_twister();
    /* is_min(i) = \pi(i) < \min_{j \in adj(i)} \pi{j} */ 
    for (i = 0; i < nneigh; ++i) {
      COMM_PACK(in_Vp, neigh[i]);
      COMM_PACK(pi, neigh[i]);
    }
    comm_exch();
    is_min = 1;
    while (comm_recv()) {
      COMM_UNPACK(neigh_in_Vp);
      COMM_UNPACK(neighpi);
      if (neigh_in_Vp && (!(pi < neighpi)))
        is_min = 0;
    }
    /* Ip = { i \in V' | is_min(i) } */
    in_Ip = in_Vp && is_min;
    /* I = I \cup I' */
    in_I = in_I || in_Ip;
    /* Y = I' \cup N(I') */
    for (i = 0; i < nneigh; ++i)
      COMM_PACK(in_Ip, neigh[i]);
    comm_exch();
    in_Y = in_Ip;
    while (comm_recv()) {
      COMM_UNPACK(neigh_in_Ip);
      if (in_Vp && neigh_in_Ip)
        in_Y = 1;
    }
    /* V' = V' - Y */
    in_Vp = in_Vp && (!in_Y);
  }
  /* optional verification round.
     keep this around until this
     comment is quite old */
  for (i = 0; i < nneigh; ++i)
    COMM_PACK(in_I, neigh[i]);
  comm_exch();
  any_neigh_in_I = 0;
  while (comm_recv()) {
    COMM_UNPACK(neigh_in_I);
    for (i = 0; i < nneigh; ++i) {
      if (neigh[i] == comm_from())
        break;
      if (neigh[i] == comm_rank())
        die("luby_mis: %d is neighbor with self\n", comm_rank());
    }
    if (i == nneigh)
      die("luby_mis: graph not symmetric. %d unexpected neighbor %d\n",
          comm_rank(), comm_from());
    if (in_I && neigh_in_I)
      die("luby_mis: bug: not independent. %d and %d adjacent and in set\n",
          comm_rank(), comm_from());
    if (neigh_in_I)
      any_neigh_in_I = 1;
  }
  if ((!any_neigh_in_I) && (!in_I) && in_original_Vp)
    die("luby_mis: bug: not maximal. %d could have been taken but wasn't\n",
        comm_rank());
  /* end verification */
  return in_I;
}

unsigned luby_color(unsigned nneigh, int const neigh[])
{
  unsigned color;
  unsigned mycolor = 0; /* silences warning only */
  int colored;
  int in_set;
  colored = 0;
  color = 0;
  while (mpi_max_int(comm_mpi(), !colored)) {
    in_set = luby_mis(nneigh, neigh, !colored);
    if (in_set) {
      mycolor = color;
      colored = 1;
    }
    ++color;
  }
  return mycolor;
}

unsigned luby_color_mesh_parts(mesh* m)
{
  unsigned nneigh;
  int* neigh;
  unsigned i;
  unsigned color;
  rpeer rp;
  nneigh = rpeer_count(m);
  neigh = my_malloc(sizeof(int) * nneigh);
  i = 0;
  for (rp = rpeer_f(m); rpeer_ok(rp); rp = rpeer_n(m, rp))
    neigh[i++] = rpeer_rank(m, rp);
  color = luby_color(nneigh, neigh);
  my_free(neigh);
  return color;
}

color luby_color_from_index(unsigned ci)
{
  unsigned ncolors;
  unsigned i;
  ncolors = mpi_max_unsigned(comm_mpi(), ci) + 1;
  /* as long as all ranks give the same seed,
     they should get the same color array */
  mersenne_twister_seed(42);
  for (i = 0; i < ci; ++i)
    mersenne_twister_color();
  return mersenne_twister_color();
}
