#include "socket_w32.h"
#include "../basics.h"
#include "../client.h"

/* we're not going to do anything to
   separate the whole WSADATA from
   the individual client, just be aware
   of that. */
struct client {
  WSADATA wsa_dat;
  SOCKET s;
};

client* client_new(char const* servname, int port)
{
  client* c;
  struct addrinfo* result = NULL;
  struct addrinfo hints;
  int err;
  c = my_malloc(sizeof(*c));
  err = WSAStartup(MAKEWORD(2, 2), &c->wsa_dat);
  ASSERT(!err);
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  err = sane_getaddrinfo(servname, port, &hints, &result);
  ASSERT(!err);
  c->s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  ASSERT(c->s != INVALID_SOCKET);
  err = connect(c->s, result->ai_addr, (int)(result->ai_addrlen));
  ASSERT(err != SOCKET_ERROR);
  freeaddrinfo(result);
  return c;
}

void client_free(client* c)
{
  closesocket(c->s);
  WSACleanup();
  my_free(c);
}

int client_try_send(client* c, void const* data, unsigned size)
{
  return try_send(c->s, data, size);
}

int client_try_recv(client* c, void* data, unsigned size)
{
  return try_recv(c->s, data, size);
}
