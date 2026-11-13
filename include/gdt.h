#ifndef GDT_H
#define GDT_H

#include "types.h"

#define GDT_ADDR		0x800
#define GDT_ENTRIES		7
#define GDT_KERNEL_CODE		0x08
#define GDT_KERNEL_DATA		0x10
#define GDT_KERNEL_STACK	0x18
#define GDT_USER_CODE		0x23
#define GDT_USER_DATA		0x2B
#define GDT_USER_STACK		0x33

struct gdt_entry {
	u16	limit_low;
	u16	base_low;
	u8	base_mid;
	u8	access;
	u8	granularity;   /* high nibble: flags, low nibble: limit bits 16-19 */
	u8	base_high;
}	__attribute__((packed));

struct gdt_ptr {
	u16		limit;
	u32		base;
}	__attribute__((packed));

void		gdt_init(void);

#endif
