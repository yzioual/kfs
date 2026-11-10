#include "vga.h"
#include "types.h"
#include "string.h"

static u16 *const vga_buf = (u16 *)VGA_ADDR;
static size_t row;
static size_t col;
static u8 color = 0x0F;

static inline u16 vga_entry(char c, u8 attr)
{
    return ((u16)attr << 8) | (u8)c;
}

static void vga_scroll(void)
{
    ft_memmove(vga_buf,
            vga_buf + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(u16));
    size_t i;
    for (i = 0; i < VGA_WIDTH; i++)
        vga_buf[VGA_HEIGHT - 1] = vga_entry(' ', color);
}

void vga_putchar(char c)
{
    if (c == '\n') {
        col = 0;
        row += 1;
    } else {
        vga_buf[row * VGA_WIDTH + col] = vga_entry(c, color);
	col += 1;
    }

    if (col >= VGA_WIDTH) {
        col = 0;
        row += 1;
    }

    if (row >= VGA_HEIGHT) {
        vga_scroll();
        row = VGA_HEIGHT - 1;
    }
}

void vga_puts(const char *s)
{
    int i = -1;
    while (s[++i])
        vga_putchar(s[i]);
}

void vga_init(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buf[i] = vga_entry(' ', VGA_WHITE_ON_BLACK);
}
