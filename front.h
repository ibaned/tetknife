#ifndef FRONT_H
#define FRONT_H

void front_start(int argc, char** argv);
void front_stop(void);

unsigned char* front_pixels(void);
unsigned front_width(void);
unsigned front_height(void);

void front_mouse_down(double x, double y);
void front_mouse_up(double x, double y);
void front_key_down(char k);
void front_key_up(void);

#endif
