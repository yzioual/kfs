#include "vga.h"
#include "types.h"
#include "string.h"
#include "io.h"

static u16 *const vga_buf = (u16 *)VGA_ADDR;
static size_t row;
static size_t col;
static u8 color = 0x0F;

static void vga_update_cursor(void)
{
    u16 pos = row * VGA_WIDTH + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}

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
		vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(' ', color);
}

void vga_putchar(char c)
{
	if (c == '\b') {
		if (col > 0)
			col--;
		else if (row > 0) {
			row--;
			col = VGA_WIDTH - 1;
		}
		vga_buf[row * VGA_WIDTH + col] = vga_entry(' ', color);
	} else if (c == '\n') {
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
	vga_update_cursor();
}

void vga_puts(const char *s)
{
	int i = -1;
	while (s[++i])
		vga_putchar(s[i]);
}

void vga_init(void)
{
	row = 0;
	col = 0;

	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga_buf[i] = vga_entry(' ', VGA_WHITE_ON_BLACK);
	vga_update_cursor();
}

void vga_set_color(t_vga_color fg, t_vga_color bg)
{
	color = ((bg & 0x07) << 4) | (fg & 0x0F);
}

void vga_save(u16 *buf, size_t *r, size_t *c, u8 *col_attr)
{
	ft_memcpy(buf, vga_buf, (VGA_WIDTH * VGA_HEIGHT * sizeof(u16)));
	*r = row;
	*c = col;
	*col_attr = color;
}

void vga_restore(const u16 *buf, size_t r, size_t c, u8 col_attr)
{
	ft_memcpy(vga_buf, buf, (VGA_WIDTH * VGA_HEIGHT * sizeof(u16)));
	row = r;
	col = c;
	color = col_attr;
	vga_update_cursor();
}
