#ifndef RIB_H
#define RIB_H

#include "space.h"

typedef struct {
  point p;
  int i;
  int padding_;
} rib_obj;

void rib_sort(unsigned n, rib_obj o[]);

#endif
