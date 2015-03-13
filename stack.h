#ifndef STACK_H
#define STACK_H

#include "basics.h"

typedef struct {
  unsigned n;
  unsigned c;
} stack;

void stack_init(stack* s);
void stack_clear(stack* s);
int stack_full(stack* s);
unsigned stack_grow(stack* s);
int stack_push(stack* s);
int stack_f(stack* s);
int stack_n(stack* s, int i);

#endif
