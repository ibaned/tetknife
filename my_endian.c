#include "my_endian.h"

static unsigned const canary = 1;

#define ENDIANNESS (*((unsigned char*)&canary))
#define LITTLE_ENDIAN 1

static void swap(void* out, void const* in, unsigned size)
{
  unsigned char* a;
  unsigned char const* b;
  unsigned i;
  a = out;
  b = in;
  for (i = 0; i < size; ++i)
    a[size - i - 1] = b[i];
}

unsigned my_htonl(unsigned x)
{
  unsigned out;
  if (ENDIANNESS == LITTLE_ENDIAN)
    swap(&out, &x, sizeof(x));
  else
    out = x;
  return out;
}

double my_htond(double x)
{
  double out;
  if (ENDIANNESS == LITTLE_ENDIAN)
    swap(&out, &x, sizeof(x));
  else
    out = x;
  return out;
}
