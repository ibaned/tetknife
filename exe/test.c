#include "../back.h"
#include "../basics.h"

#define WIDTH 640
#define HEIGHT 480
static unsigned char pixels[WIDTH * HEIGHT * 3] = {0}; 

void back_start(void)
{
}

void back_stop(void)
{
}

unsigned char* back_pixels(void)
{
  debug("pixels\n");
  return pixels;
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
  debug("mouse_down(%f, %f)\n", x, y);
}

void back_mouse_up(double x, double y)
{
  debug("mouse_up(%f, %f)\n", x, y);
}

void back_key_down(char k)
{
  debug("key_down(%c)\n", k);
}

void back_key_up(void)
{
  debug("key_up\n");
}
