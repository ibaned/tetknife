#include "../back.h"
#include "../draw.h"
#include "../comm.h"
#include "../luby.h"

#define WIDTH 640
#define HEIGHT 480
static drawing global_drawing;

void back_start(void)
{
  triangle t;
  drawing_init(&global_drawing, WIDTH, HEIGHT);
  if (comm_rank() == 0) {
    t.a = point_new(3,4,-5);
    t.b = point_new(200,10,-5);
    t.c = point_new(7,200,-4);
  } else if (comm_rank() == 1) {
    t.a = point_new(8,9,-2);
    t.b = point_new(200,3,-4);
    t.c = point_new(8,8,-200);
  } else if (comm_rank() == 2) {
    t.a = point_new(8,20,-11);
    t.b = point_new(8,200,-7);
    t.c = point_new(9,10,-200);
  } else if (comm_rank() == 3) {
    t.a = point_new(200,4,-5);
    t.a = point_new(9,200,-1);
    t.a = point_new(10,11,-200);
  }
  drawing_clear(&global_drawing, black);
  draw_triangle(&global_drawing, t, luby_color_from_index((unsigned)comm_rank()));
  drawing_reduce(&global_drawing);
}

void back_stop(void)
{
  drawing_dtor(&global_drawing);
}

unsigned char* back_pixels(void)
{
  return (unsigned char*)(global_drawing.im.p[0]);
}

unsigned back_width(void)
{
  return WIDTH;
}

unsigned back_height(void)
{
  return HEIGHT;
}

void back_mouse_down(double x, double y)
{
  (void)x;
  (void)y;
}

void back_mouse_up(double x, double y)
{
  (void)x;
  (void)y;
}

void back_key_down(char k)
{
  (void)k;
}

void back_key_up(void)
{
}

