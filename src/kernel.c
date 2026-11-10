#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_WHITE_ON_BLACK 0x0F

void kmain(unsigned int magic, unsigned int info)
{
	volatile unsigned short *vga = (volatile unsigned short *) VGA_ADDR;
	unsigned int i;

	(void) magic;
	(void)info;

	for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga[i] = (VGA_WHITE_ON_BLACK << 8 || ' ');

	vga[0] = (VGA_WHITE_ON_BLACK << 8) | '4';
	vga[1] = (VGA_WHITE_ON_BLACK << 8) | '2';

	for (;;) // halt forever
		;
}
