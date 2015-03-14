#ifndef FLAG_H
#define FLAG_H

typedef struct {
  unsigned char* bytes;
} flag;

void flag_init(flag* f);
void flag_grow(flag* f, unsigned c);
void flag_clear(flag* f, int i);
void flag_set(flag* f, int i);
int flag_get(flag* f, int i);
void flag_free(flag* f);

#endif
