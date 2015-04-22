#include "front.h"
#include "server.h"
#include "image.h"
#include "my_endian.h"
#include "basics.h"
#include "socket_codes.h"

static server* the_server = 0;
static image the_image = {0,0,0};
static int initialized = 0;

static unsigned recv_unsigned(server* s)
{
  unsigned x;
  ASSERT(server_try_recv(s, &x, sizeof(x)));
  x = my_htonl(x);
  return x;
}

static void recv_image(server* s, image* im)
{
  unsigned w, h;
  w = recv_unsigned(s);
  h = recv_unsigned(s);
  if (!initialized) {
    image_init(im, w, h);
    initialized = 1;
  } else {
    ASSERT(w == im->w);
    ASSERT(h == im->h);
  }
  ASSERT(server_try_recv(s, im->p[0],
        im->w * im->h * sizeof(color)));
}

static void send_char(server* s, char c)
{
  ASSERT(server_try_send(s, &c, sizeof(c)));
}

static void send_code(server* s, socket_code c)
{
  send_char(s, c);
}

static void send_double(server* s, double x)
{
  x = my_htond(x);
  ASSERT(server_try_send(s, &x, sizeof(x)));
}

void front_start(void)
{
  static int const port = 4242;
  the_server = server_new(port);
  print("listening on port %d ...\n", port);
  server_accept(the_server);
  print("got a connection\n");
  recv_image(the_server, &the_image);
}

void front_stop(void)
{
  send_code(the_server, SOCKET_QUIT);
  image_dtor(&the_image);
  server_free(the_server);
}

unsigned char* front_pixels(void)
{
  return (unsigned char*) the_image.p[0];
}

unsigned front_width(void)
{
  return the_image.w;
}

unsigned front_height(void)
{
  return the_image.h;
}

void front_mouse_down(double x, double y)
{
  send_code(the_server, SOCKET_MOUSE_DOWN);
  send_double(the_server, x);
  send_double(the_server, y);
}

void front_mouse_up(double x, double y)
{
  send_code(the_server, SOCKET_MOUSE_UP);
  send_double(the_server, x);
  send_double(the_server, y);
  recv_image(the_server, &the_image);
}

void front_key_down(char k)
{
  send_code(the_server, SOCKET_KEY_DOWN);
  send_char(the_server, k);
}

void front_key_up(void)
{
  send_code(the_server, SOCKET_KEY_UP);
  recv_image(the_server, &the_image);
}
