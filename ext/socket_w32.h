/* apparently getaddrinfo is only available
   if we tell these headers that the world
   has moved past Win95 */
#define WINVER 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
/* sprintf for sane_getaddrinfo */
#include <stdio.h>

static int sane_getaddrinfo(char const* servname, int port,
    struct addrinfo* hints, struct addrinfo** result)
{
  char buf[32];
  sprintf(buf, "%d", port);
  return getaddrinfo(servname, buf, hints, result);
}

static int try_send(SOCKET s, void const* data, unsigned size)
{
  char const* p;
  int step;
  p = data;
  while (size) {
    step = send(s, p, size, 0);
    if (step == SOCKET_ERROR) {
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        continue;
      else
        return 0;
    } else {
      size -= step;
      p += step;
    }
  }
  return 1;
}

static int try_recv(SOCKET s, void* data, unsigned size)
{
  char* p;
  int step;
  p = data;
  while (size) {
    step = recv(s, p, size, 0);
    if (step == SOCKET_ERROR) {
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        continue;
      else
        return 0;
    } else {
      size -= step;
      p += step;
    }
  }
  return 1;
}

