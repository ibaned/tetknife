#include "socket_w32.h"
#include "../basics.h"
#include "../server.h"

/* we're not going to do anything to
   separate the whole WSADATA from
   the individual server, just be aware
   of that. */
struct server {
  WSADATA wsa_dat;
  SOCKET listen_s;
  SOCKET s;
};

server* server_new(int port)
{
  server* s;
  struct addrinfo* result = NULL;
  struct addrinfo hints;
  int err;
  s = my_malloc(sizeof(*s));
  err = WSAStartup(MAKEWORD(2, 2), &s->wsa_dat);
  ASSERT(!err);
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  err = sane_getaddrinfo(NULL, port, &hints, &result);
  ASSERT(!err);
  s->listen_s = socket(result->ai_family, result->ai_socktype,
      result->ai_protocol);
  ASSERT(s->listen_s != INVALID_SOCKET);
  err = bind(s->listen_s, result->ai_addr, (int) (result->ai_addrlen));
  ASSERT(err != SOCKET_ERROR);
  freeaddrinfo(result);
  err = listen(s->listen_s, 1);
  ASSERT(err != SOCKET_ERROR);
  s->s = INVALID_SOCKET;
  return s;
}

void server_accept(server* s)
{
  s->s = accept(s->listen_s, NULL, NULL);
  ASSERT(s->s != INVALID_SOCKET);
  closesocket(s->listen_s);
  s->listen_s = INVALID_SOCKET;
}

void server_free(server* s)
{
  closesocket(s->s);
  WSACleanup();
}

int server_try_send(server* s, void const* data, unsigned size)
{
  return try_send(s->s, data, size);
}

int server_try_recv(server* s, void* data, unsigned size)
{
  return try_recv(s->s, data, size);
}
