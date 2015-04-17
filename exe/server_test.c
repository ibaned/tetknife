#include "../server.h"
#include "../basics.h"

int main()
{
  server* s;
  unsigned msg;
  s = server_new(4242);
  server_accept(s);
  server_try_recv(s, &msg, sizeof(msg));
  print("server got %u\n", msg);
  msg += 24;
  print("server sending %u\n", msg);
  server_try_send(s, &msg, sizeof(msg));
  server_free(s);
  return 0;
}

