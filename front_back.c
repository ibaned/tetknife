#include "front.h"
#include "back.h"

void front_start(void)
{
  back_start();
}

void front_stop(void)
{
  back_stop();
}

unsigned char* front_pixels(void)
{
  return back_pixels();
}

unsigned front_width(void)
{
  return back_width();
}

unsigned front_height(void)
{
  return back_height();
}

void front_mouse_down(double x, double y)
{
  back_mouse_down(x, y);
}

void front_mouse_up(double x, double y)
{
  back_mouse_up(x, y);
}

void front_key_down(char k)
{
  back_key_down(k);
}

void front_key_up(void)
{
  back_key_up();
}
