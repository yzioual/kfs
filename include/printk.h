#ifndef PRINTK_H
#define PRINTK_H

#include "types.h"
#include "vga.h"
#include <stdarg.h>

int	printk(const char *format, ...);

#endif
