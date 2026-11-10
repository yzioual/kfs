#include "vga.h"

void kmain(unsigned int magic, unsigned int info)
{
	(void) magic;
	(void)info;

	vga_init();
	vga_puts("42\n");

	for (;;) // halt forever
		;
}
