#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "screen.h"
#include "gdt.h"

void kmain(unsigned int magic, unsigned int info)
{
	(void) magic;
	(void)info;

	vga_init();
	gdt_init();
	screen_init();

	vga_set_color(VGA_RED, VGA_BLUE);
	vga_puts("42\n");

	printk("hello\n");
	printk("%d %d %d\n", 0, -1, 2147483647);      /* INT_MIN would be better still */
	printk("%x %X %p\n", 0xdeadbeef, 0xdeadbeef, (void *)0x800);
	printk("%s %c %%\n", "str", 'A');
	printk(NULL);                                   /* should not crash */
	printk("%d%s%x", 1, "two", 3);                  /* no trailing newline */

/*
	vga_set_color(VGA_YELLOW, VGA_BLUE);
	vga_puts("Yellow on blue\n");

	vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
	vga_puts("Green on black\n");


	vga_set_color(VGA_WHITE, VGA_BLACK);
	printk("printk test: str=%s int=%d hex=%x ptr=%p\n", "hello", 42, 0xDEAD, &magic);
	printk("unsigned=%u negative=%d char=%c percent=%%\n", 123456u, -42, 'Z');
*/
	while (true) {
		char x = keyboard_getchar();
		if (x)
			printk("%c", x);
	}

	for (;;) // halt forever
		;
}
