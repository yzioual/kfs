#include "vga.h"
#include "printk.h"

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

	vga_set_color(VGA_WHITE, VGA_BLACK);
	printk("printk test: str=%s int=%d hex=%x ptr=%p\n", "hello", 42, 0xDEAD, &magic);
	printk("unsigned=%u negative=%d char=%c percent=%%\n", 123456u, -42, 'Z');

	for (;;) // halt forever
		;
}
