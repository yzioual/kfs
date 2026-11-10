#include "vga.h"

void kmain(unsigned int magic, unsigned int info)
{
	(void) magic;
	(void)info;

	vga_init();
	vga_set_color(VGA_RED, VGA_BLUE);
	vga_puts("42\n");
	vga_set_color(VGA_YELLOW, VGA_BLUE);
	vga_puts("Yellow on blue\n");

	vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
	vga_puts("Green on black\n");

	vga_set_color(VGA_WHITE, VGA_RED);
	vga_puts("White on red\n");

	for (;;) // halt forever
		;
}
