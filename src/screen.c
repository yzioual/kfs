#include "screen.h"
#include "types.h"
#include "vga.h"
#include "string.h"

static t_screen screens[SCREEN_COUNT];
static int      current;

void screen_init(void)
{
	for (int i = 0; i < SCREEN_COUNT; i++)
	{
		for (int j = 0; j < VGA_WIDTH * VGA_HEIGHT; j++)
			screens[i].buf[j] = ((u16)VGA_WHITE_ON_BLACK << 8) | ' ';
		screens[i].col = 0;
		screens[i].row = 0;
		screens[i].color = VGA_WHITE_ON_BLACK;

	}
	current = 0;
}

void screen_switch(int n)
{
	if (n < 0 || n >= SCREEN_COUNT || n == current) return;
	vga_save(screens[current].buf, &screens[current].row, &screens[current].col, &screens[current].color);
	current = n;
	vga_restore(screens[current].buf, screens[current].row, screens[current].col, screens[current].color);
}

int screen_get_current(void)
{
	return current;
}
