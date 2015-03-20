#include "image.h"
#include "basics.h"

color const black = {0,0,0};
color const white = {255,255,255};
color const red = {255,0,0};
color const green = {0,255,0};
color const blue = {0,0,255};
color const yellow = {255,255,0};

void image_init(image* im, unsigned w, unsigned h)
{
  unsigned i;
  im->w = w;
  im->h = h;
  im->p = my_malloc(h * sizeof(color*));
  im->p[0] = my_malloc(w * h * sizeof(color));
  for (i = 1; i < h; ++i)
    im->p[i] = im->p[i - 1] + w;
}

void image_dtor(image* im)
{
  my_free(im->p[0]);
  my_free(im->p);
}

void image_clear(image* im, color c)
{
  unsigned i;
  unsigned j;
  for (i = 0; i < im->h; ++i)
    for (j = 0; j < im->w; ++j)
      im->p[i][j] = c;
}
