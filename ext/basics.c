#include "../basics.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <complex.h>

void print(char const* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stdout, format, ap);
  va_end(ap);
}

void debug(char const* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}

void die(char const* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  abort();
}

void my_assert_fail(char const* cond, char const* file, int line)
{
  die("xmesh assert fail: '%s' %s:%i\n", cond, file, line);
}

void* my_malloc(unsigned long bytes)
{
  return malloc(bytes);
}

void* my_realloc(void* p, unsigned long bytes)
{
  return realloc(p, bytes);
}

void my_free(void* p)
{
  free(p);
}

double my_sin(double x)
{
  return sin(x);
}

double my_cos(double x)
{
  return cos(x);
}

double my_atan2(double y, double x)
{
  return atan2(y, x);
}

double my_sqrt(double x)
{
  return sqrt(x);
}

double my_pow(double x, double e)
{
  return pow(x, e);
}

double const my_dbl_max = DBL_MAX;

/*
http://en.wikipedia.org/wiki/Cubic_function#The_nature_of_the_roots
*/

unsigned cubic_roots(double a, double b, double c, double d, double x[])
{
  double del, del0, del1;
  double t0;
  double complex t1;
  double complex C;
  double complex u[3];
  double complex cx[3];
  double complex bx;
  unsigned i;
  del0 = b * b - 3 * a * c;
  del1 = 2 * b * b * b - 9 * a * b * c + 27 * a * a * d;
  t0 = del1 * del1 - 4 * del0 * del0 * del0;
  del = t0 / (-27.0 * a * a);
  u[0] = 1.0;
  u[1] = (-1.0 + sqrt(3) * I) / 2.0;
  u[2] = (-1.0 - sqrt(3) * I) / 2.0;
  if (del && !del0)
    t1 = del1;
  else
    t1 = csqrt(t0);
  C = cpow((del1 + t1) / 2.0, 1.0 / 3.0);
  for (i = 0; i < 3; ++i) {
    if (C)
      cx[i] = -(b + u[i] * C + del0 / (u[i] * C)) / (3 * a);
    else
      cx[i] = -b / (3 * a);
  }
  if (del < 0) {
    bx = cx[0];
    for (i = 1; i < 3; ++i)
      if (fabs(cimag(cx[i])) < fabs(cimag(bx)))
        bx = cx[i]; /* choose least imaginary root */
    x[0] = creal(bx);
    return 1;
  } else {
    for (i = 1; i < 3; ++i)
      x[i] = creal(cx[i]);
    return 3;
  }
}
