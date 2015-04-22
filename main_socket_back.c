#include "back.h"
#include "client.h"
#include "my_endian.h"
#include "basics.h"
#include "socket_codes.h"
#include "comm.h"
#include "ibarrier.h"

static void send_unsigned(client* c, unsigned x)
{
  x = my_htonl(x);
  ASSERT(client_try_send(c, &x, sizeof(x)));
}

static void send_image(client* c)
{
  unsigned w, h;
  if (comm_rank() != 0)
    return;
  w = back_width();
  h = back_height();
  send_unsigned(c, w);
  send_unsigned(c, h);
  ASSERT(client_try_send(c, back_pixels(), w * h * 3));
}

static char just_recv_char(client* c)
{
  char x;
  ASSERT(client_try_recv(c, &x, sizeof(x)));
  return x;
}

/* MPICH uses busy polling in its barrier implementation,
   and that tends to max out N-1 CPUs when waiting for
   the next user event. this uses our ibarrier and
   a sleep to reduce that stress for this particular barrier */
static void lazy_barrier(void)
{
  ibarrier_begin(mpi_world());
  while (!ibarrier_done())
    /* rank 0 is the holdup, so shouldn't
       spend long in the ibarrier.
       this also prevents calling client_sleep in serial */
    if (comm_rank() != 0)
      client_sleep(50);
}

static socket_code recv_code(client* c)
{
  socket_code c2;
  if (comm_rank() == 0)
    c2 = (socket_code) (just_recv_char(c));
  lazy_barrier();
  mpi_bcast(mpi_world(), &c2, sizeof(c2));
  return c2;
}

static char recv_char(client* c)
{
  char x;
  if (comm_rank() == 0)
    x = just_recv_char(c);
  mpi_bcast(mpi_world(), &x, sizeof(x));
  return x;
}

static double recv_double(client* c)
{
  double x;
  if (comm_rank() == 0) {
    ASSERT(client_try_recv(c, &x, sizeof(x)));
    x = my_htond(x);
  }
  mpi_bcast(mpi_world(), &x, sizeof(x));
  return x;
}

int main()
{
  client* the_client = 0;
  mpi_init();
  comm_init(mpi_world());
  if (comm_rank() == 0)
    the_client = client_new("127.0.0.1", 4243);
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
  if (comm_rank() == 0)
    client_free(the_client);
  comm_finalize();
  mpi_finalize();
  return 0;
}
