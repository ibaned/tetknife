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
int stack_can_push(stack* s, unsigned n);
unsigned stack_grow_by(stack* s, unsigned n);
int stack_can_hold(stack* s, unsigned n);
unsigned stack_grow_to(stack* s, unsigned n);
int stack_push(stack* s);
int stack_push_by(stack* s, unsigned n);
int stack_f(stack* s);
int stack_n(stack* s, int i);

#endif
