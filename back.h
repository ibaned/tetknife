#ifndef BACK_H
#define BACK_H

void back_start(void);
void back_stop(void);

unsigned char* back_pixels(void);
unsigned back_width(void);
unsigned back_height(void);

void back_mouse_down(double x, double y);
void back_mouse_up(double x, double y);
void back_key_down(char k);
void back_key_up(void);

#endif
