#ifndef DRAW_H
#define DRAW_H

#include "image.h"
#include "simplex.h"

typedef struct {
  image im;
  double** z;
} drawing;

void drawing_init(drawing* d, unsigned w, unsigned h);
void drawing_dtor(drawing* d);

void drawing_clear(drawing* d, color c);

void draw_point(drawing* d, point p, color c);
void draw_line(drawing* d, line l, color c);
void draw_triangle(drawing* d, triangle t, color c);
void draw_text(drawing* dr, const char* s, point p, color c);

#endif
