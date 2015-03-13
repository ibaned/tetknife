#include "../cad.h"

int main()
{
  cad* c;
  gent e;
  c = cad_new();
  e = gent_new(c, CAD_VERTEX);
  gent_free(c, e);
  cad_free(c);
  return 0;
}
