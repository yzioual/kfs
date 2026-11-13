#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "screen.h"
#include "gdt.h"
#include "shell.h"

void kmain(unsigned int magic, unsigned int info)
{
	(void)magic;
	(void)info;

	vga_init();
	gdt_init();
	screen_init();

	shell_run();

	for (;;)
		__asm__ volatile ("hlt");
}
