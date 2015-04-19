#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include "image.h"

void mersenne_twister_seed(unsigned seed);
unsigned mersenne_twister(void);
double mersenne_twister_real(void);
color mersenne_twister_color(void);

#endif
