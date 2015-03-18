#ifndef BASICS_H
#define BASICS_H

void debug(char const* format, ...)
  __attribute__((format(printf, 1, 2)));
void die(char const* why, ...)
  __attribute__((noreturn,format(printf,1,2)));
void my_assert_fail(char const* cond, char const* file, int line)
  __attribute__((noreturn));

#define ASSERT(c) ((c)?((void)0):my_assert_fail(#c,__FILE__,__LINE__))

void* my_malloc(unsigned long bytes);
void* my_realloc(void* p, unsigned long bytes);
void my_free(void* p);

#define REALLOC(p,n) ((p)=my_realloc(p,(n)*sizeof(*(p))))

enum { NULL_IDX = -1 };

#define IDX_OK(i) ((i) >= 0)

double my_sin(double x);
double my_cos(double x);
double my_atan2(double y, double x);
double my_sqrt(double x);

extern double const my_dbl_max;

#define MIN(a,b) (((b)<(a))?(b):(a))
#define MAX(a,b) (((b)>(a))?(b):(a))

#endif
