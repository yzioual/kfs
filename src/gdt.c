#include "gdt.h"

extern void gdt_flush(u32 gdt_ptr_addr);

static struct gdt_entry *const gdt = (struct gdt_entry *)GDT_ADDR;
static struct gdt_ptr gp;

static void gdt_set(int i, u32 base, u32 limit, u8 access, u8 flags)
{
	gdt[i].base_low = base & 0xFFFF;
	gdt[i].base_mid = (base >> 16) & 0xFF;
	gdt[i].base_high = (base >> 24) & 0xFF;
	gdt[i].limit_low =  limit & 0xFFFF;
	gdt[i].granularity = (u8)(((limit >> 16) & 0x0F) | ((flags & 0x0F) << 4));
	gdt[i].access = access;
}

void gdt_init(void)
{
	gp.limit = sizeof(struct gdt_entry) * (GDT_ENTRIES) - 1;
	gp.base = GDT_ADDR;
	gdt_set(0, 0, 0, 0, 0);
	gdt_set(1, 0, 0xFFFFF, 0x9A, 0xC);
	gdt_set(2, 0, 0xFFFFF, 0x92, 0xC);
	gdt_set(3, 0, 0xFFFFF, 0x92, 0xC);
	gdt_set(4, 0, 0xFFFFF, 0xFA, 0xC);
	gdt_set(5, 0, 0xFFFFF, 0xF2, 0xC);
	gdt_set(6, 0, 0xFFFFF, 0xF2, 0xC);
	gdt_flush((u32) &gp);
}
