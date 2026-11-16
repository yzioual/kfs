#include "vga.h"
#include "printk.h"
#include "keyboard.h"
#include "screen.h"
#include "gdt.h"
#include "shell.h"

extern void setup_paging(void);
extern void load_pd(void);
extern void enable_paging(void);

void kmain(unsigned int magic, unsigned int info)
{
	(void)magic;
	(void)info;

	vga_init();
	gdt_init();

	setup_paging();
	load_pd();
	enable_paging();

	screen_init();
	shell_run();

	//*(volatile unsigned short *)0xB8000 = 0x0F50; /* White P */

	for (;;)
		__asm__ volatile ("hlt");
}
