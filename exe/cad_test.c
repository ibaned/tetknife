#include "../cad.h"
#include "../cad_gen.h"
#include "../cad_geom.h"

int main()
{
  point const pts[4] = {
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
  };
  cad* c;
  c = cad_new();
  geom_new(c);
  cad_gen_polygon(c, 4, pts);
  cad_free(c);
  return 0;
}
