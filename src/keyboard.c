#include "io.h"
#include "kb.h"
#include "printk.h"

#define KEYBOARD_DATA			0x60
#define KEYBOARD_STATUS			0x64

#define RSHIFT_IS_PRESSED		0x2A
#define LSHIFT_IS_PRESSED		0x36
#define RSHIFT_IS_RELEASED		0xAA
#define LSHIFT_IS_RELEASED		0xB6
#define CAPS_TOGGLE			0x3A

static const char us_keymap[58] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0,'*', 0,' '
};

static const char us_keymap_shift[58] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0,
    '|','Z','X','C','V','B','N','M','<','>','?', 0,'*', 0,' '
};

static int shift;
static int caps_lock;

static u8 keyboard_read_scancode(void)
{
	(void) shift;
	(void) caps_lock;
	(void) us_keymap;

	while (!(inb(KEYBOARD_STATUS) & 1))
		; // halt until something hits the mailbox!

	return (inb(KEYBOARD_DATA));
}

char keyboard_getchar(void)
{
	u8 scancode = keyboard_read_scancode();

	if (scancode == RSHIFT_IS_PRESSED || scancode == LSHIFT_IS_PRESSED)
		shift = 1;

	if (scancode == RSHIFT_IS_RELEASED || scancode == LSHIFT_IS_RELEASED)
		shift = 0;

	if (scancode == CAPS_TOGGLE)
	{
		caps_lock = !caps_lock;
		return 0;
	}

	if (scancode & 0x80)
		return 0;

	if (scancode >= (sizeof(us_keymap)))
		return 0;

	return (shift || caps_lock) ? us_keymap_shift[scancode] : us_keymap[scancode];
}
