#include "back.h"
#include "client.h"
#include "my_endian.h"
#include "basics.h"
#include "socket_codes.h"

static void send_unsigned(client* c, unsigned x)
{
  x = my_htonl(x);
  ASSERT(client_try_send(c, &x, sizeof(x)));
}

static void send_image(client* c)
{
  unsigned w, h;
  w = back_width();
  h = back_height();
  send_unsigned(c, w);
  send_unsigned(c, h);
  ASSERT(client_try_send(c, back_pixels(), w * h * 3));
}

static char recv_char(client* c)
{
  char x;
  ASSERT(client_try_recv(c, &x, sizeof(x)));
  return x;
}

static socket_code recv_code(client* c)
{
  socket_code c2;
  c2 = (socket_code) (recv_char(c));
  return c2;
}

static double recv_double(client* c)
{
  double x;
  ASSERT(client_try_recv(c, &x, sizeof(x)));
  x = my_htond(x);
  return x;
}

int main()
{
  client* the_client;
  the_client = client_new("127.0.0.1", 4242);
  back_start();
  send_image(the_client);
  while (1)
    switch (recv_code(the_client)) {
      case SOCKET_MOUSE_DOWN:
        back_mouse_down(recv_double(the_client), recv_double(the_client));
        break;
      case SOCKET_MOUSE_UP:
        back_mouse_up(recv_double(the_client), recv_double(the_client));
        send_image(the_client);
        break;
      case SOCKET_KEY_DOWN:
        back_key_down(recv_char(the_client));
        break;
      case SOCKET_KEY_UP:
        back_key_up();
        send_image(the_client);
        break;
      case SOCKET_QUIT:
        goto done;
    }
done:
  back_stop();
  client_free(the_client);
  return 0;
}
