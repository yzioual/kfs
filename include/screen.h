#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_COUNT 3

#include "types.h"
#include "vga.h"

typedef struct s_screen {
    u16    buf[VGA_WIDTH * VGA_HEIGHT];
    size_t row;
    size_t col;
    u8     color;
} t_screen;

void screen_init(void);
void screen_switch(int n);
int  screen_get_current(void);

#endif
