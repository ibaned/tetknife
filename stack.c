#include "stack.h"

void stack_init(stack* s)
{
  s->n = 0;
  s->c = 0;
}

void stack_clear(stack* s)
{
  s->n = 0;
}

int stack_full_n(stack* s, unsigned n)
{
  return s->n + n >= s->c;
}

int stack_full(stack* s)
{
  return s->n == s->c;
}

unsigned stack_grow_n(stack* s, unsigned n)
{
  stack_grow(s);
  s->c = MAX(s->c, s->n + n);
  return s->c;
}

unsigned stack_grow(stack* s)
{
  s->c = ((s->c + 1) * 3) / 2;
  return s->c;
}

int stack_push(stack* s)
{
  ASSERT(!stack_full(s));
  return (int)(s->n++);
}

static int iter(stack* s, int i)
{
  if ((unsigned)i == s->n)
    return NULL_IDX;
  return i;
}

int stack_f(stack* s)
{
  return iter(s, 0);
}

int stack_n(stack* s, int i)
{
  return iter(s, i + 1);
}
