#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color;

extern color const black;
extern color const white;
extern color const red;
extern color const green;
extern color const blue;
extern color const yellow;

typedef struct {
  unsigned w;
  unsigned h;
  color** p;
} image;

typedef struct {
  int x;
  int y;
} pix;

void image_init(image* im, unsigned w, unsigned h);
void image_dtor(image* im);

void image_clear(image* im, color c);

#endif
