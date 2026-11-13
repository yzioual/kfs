#ifndef VGA_H
#define VGA_H

#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_WHITE_ON_BLACK 0x0F

#include "types.h"

typedef enum e_vga_color {
    VGA_BLACK = 0,  VGA_BLUE,        VGA_GREEN,       VGA_CYAN,
    VGA_RED,        VGA_MAGENTA,     VGA_BROWN,       VGA_LIGHT_GREY,
    VGA_DARK_GREY,  VGA_LIGHT_BLUE,  VGA_LIGHT_GREEN, VGA_LIGHT_CYAN,
    VGA_LIGHT_RED,  VGA_LIGHT_MAGENTA, VGA_YELLOW,    VGA_WHITE
} t_vga_color;

void vga_set_color(t_vga_color fg, t_vga_color bg); 
u8   vga_get_color(void);
void vga_init(void);
void vga_clear(void);
void vga_putchar(char c);
void vga_puts(const char *s);
void vga_save(u16 *buf, size_t *r, size_t *c, u8 *col_attr);
void vga_restore(const u16 *buf, size_t r, size_t c, u8 col_attr);

#endif
