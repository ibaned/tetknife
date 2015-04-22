#ifndef VIEW_H
#define VIEW_H

#include "simplex.h"
#include "image.h"

typedef enum {
  VIEW_ROT,
  VIEW_PAN,
  VIEW_ZOOM
} view_mode;

typedef struct view view;

view* view_new(unsigned w, unsigned h);
void view_free(view* v);

void view_focus(view* v, bbox bb);

void view_rot(view* v, double spin, double tilt);
void view_zoom(view* v, double by);
void view_pan(view* v, double x, double y);

void view_set_mode(view* v, view_mode m);
void view_click(view* v, double x, double y);
void view_drag(view* v, double x, double y);

void view_clear(view* v, color c);
void view_point(view* v, point p, color c);
void view_line(view* v, line l, color c);
void view_triangle(view* v, triangle t, color c);
void view_text(view* v, const char* s, point p, color c);

image* view_image(view* v);

void view_reduce(view* v);

#endif
