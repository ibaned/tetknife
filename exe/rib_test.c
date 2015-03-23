#include "../rib.h"
#include "../basics.h"

int main()
{
  point p[4] = {
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
  };
  int idx[4] = {0,1,2,3};
  rib_sort(4, p, idx);
  debug("after 2: %d %d %d %d\n",
      idx[0], idx[1], idx[2], idx[3]);
  return 0;
}
