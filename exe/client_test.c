#include "../client.h"
#include "../basics.h"

int main()
{
  client* c;
  unsigned msg = 1991;
  c = client_new("127.0.0.1", 4242);
  print("client sending %u\n", msg);
  client_try_send(c, &msg, sizeof(msg));
  client_try_recv(c, &msg, sizeof(msg));
  print("client got %u\n", msg);
  client_free(c);
  return 0;
}
