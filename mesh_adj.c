#include "mesh_adj.h"

void mset_init(mset* s)
{
  stack_init(&s->s);
  s->e = 0;
}

void mset_dtor(mset* s)
{
  my_free(s->e);
}

void mset_clear(mset* s)
{
  stack_clear(&s->s);
}

void mset_add(mset* s, ment e)
{
  if (stack_full(&s->s))
    REALLOC(s->e, stack_grow(&s->s));
  s->e[s->s.n] = e;
  stack_push(&s->s);
}

int mset_has(mset* s, ment e)
{
  unsigned i;
  for (i = 0; i < s->s.n; ++i)
    if (ment_eq(s->e[i], e))
      return 1;
  return 0;
}
