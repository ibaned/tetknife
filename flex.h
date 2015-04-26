#ifndef FLEX_H
#define FLEX_H

#include "stack.h"

typedef struct {
  stack s;
  int* fl;
  int ff;
  unsigned n;
} flex;

void flex_init(flex* f);
void flex_dtor(flex* f);
int flex_full(flex* f);
unsigned flex_grow(flex* f);
int flex_add(flex* f);
void flex_rm(flex* f, int i);
int flex_f(flex* f);
int flex_n(flex* f, int i);
int flex_exists(flex* f, int i);

#endif
