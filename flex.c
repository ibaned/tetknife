#include "flex.h"

enum { LIVE_IDX = -2 };

void flex_init(flex* f)
{
  stack_init(&f->s);
  f->fl = 0;
  f->ff = NULL_IDX;
  f->n = 0;
}

void flex_free(flex* f)
{
  my_free(f->fl);
}

int flex_full(flex* f)
{
  return f->ff == NULL_IDX && stack_full(&f->s);
}

unsigned flex_grow(flex* f)
{
  return stack_grow(&f->s);
}

int flex_add(flex* f)
{
  int i;
  ASSERT(!flex_full(f));
  if (f->ff == NULL_IDX)
    i = stack_push(&f->s);
  else {
    i = f->ff;
    f->ff = f->fl[f->ff];
  }
  f->fl[i] = LIVE_IDX;
  f->n++;
  return i;
}

void flex_rm(flex* f, int i)
{
  ASSERT(f->n);
  f->fl[i] = f->ff;
  f->ff = i;
  f->n--;
}

static int iter(flex* f, int i)
{
  for (; IDX_OK(i); i = stack_n(&f->s, i))
    if (f->fl[i] == LIVE_IDX)
      break;
  return i;
}

int flex_f(flex* f)
{
  return iter(f, stack_f(&f->s));
}

int flex_n(flex* f, int i)
{
  return iter(f, stack_n(&f->s, i));
}

