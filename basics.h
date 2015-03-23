#ifndef BASICS_H
#define BASICS_H

void print(char const* format, ...)
  __attribute__((format(printf, 1, 2)));
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
double my_pow(double x, double e);

extern double const my_dbl_max;

#define MIN(a,b) (((b)<(a))?(b):(a))
#define MAX(a,b) (((b)>(a))?(b):(a))
#define ABS(x) (((x)<0)?(-(x)):(x))

unsigned cubic_roots(double a, double b, double c, double d, double x[]);

void my_qsort(void* base, unsigned nel, unsigned width,
    int (*compar)(void const*, void const*));
void* my_bsearch(void const* key, void const* base, unsigned nel, unsigned width,
    int (*compar)(void const*, void const*));
void my_memcpy(void* dst, void const* src, unsigned n);

#endif
