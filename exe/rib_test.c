#include "../rib.h"
#include "../basics.h"

int main()
{
  point p[2] = {{1,0,0},{0,0,0}};
  int idx[2] = {0,1};
  rib_sort(1, p, idx);
  debug("after 1: %d\n", idx[0]);
  rib_sort(2, p, idx);
  debug("after 2: %d %d\n", idx[0], idx[1]);
  return 0;
}
