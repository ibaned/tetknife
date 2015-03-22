#include "../basics.h"
#include "../space.h"

static void print_roots(unsigned n, double x[])
{
  unsigned i;
  print("roots: {");
  for (i = 0; i < n; ++i)
    print(" %f", x[i]);
  print(" }\n");
}

int main()
{
  double x[3];
  unsigned n;
  basis A = {{  0.875726,  0.055288, -1.597033},
             { -1.201369, -1.358298, -1.095408},
             {  1.447017, -1.275570, -1.720548}};
  n = cubic_roots(1.0 / 4.0,
                  3.0 / 4.0,
                 -6.0 / 4.0,
                 -8.0 / 4.0,
                 x);
  print_roots(n, x);
  n = cubic_roots(2.0,
                 -3.0,
                 -3.0,
                  2.0,
                 x);
  print_roots(n, x);
  n = basis_eigenvals(A, x);
  print_roots(n, x);
  n = basis_eigenvals(basis_ident, x);
  print_roots(n, x);
  return 0;
}
