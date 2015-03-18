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

int stack_full(stack* s)
{
  return s->n == s->c;
}

unsigned stack_grow(stack* s)
{
  s->c = ((s->c + 1) * 3) / 2;
  return s->c;
}

int stack_can_push(stack* s, unsigned n)
{
  return stack_can_hold(s, s->n + n);
}

unsigned stack_grow_by(stack* s, unsigned n)
{
  return stack_grow_to(s, s->n + n);
}

int stack_can_hold(stack* s, unsigned n)
{
  return n <= s->c;
}

unsigned stack_grow_to(stack* s, unsigned n)
{
  stack_grow(s);
  s->c = MAX(s->c, n);
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
