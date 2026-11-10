#ifndef VGA_H
#define VGA_H

#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_WHITE_ON_BLACK 0x0F

void vga_init(void);
void vga_putchar(char c);
void vga_puts(const char *s);

#endif
