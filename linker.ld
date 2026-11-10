ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}

	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
