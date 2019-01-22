ENTRY(_start)

SECTIONS
{
	. = 0xC0100000;
	_kernel_start = .;

	.text ALIGN(4K): AT(ADDR(.text) - 0xC0000000)
	{
		*(.multiboot)

		*crti.*(.init)
		*crtbegin.o(.init)
		EXCLUDE_FILE (*crtbegin.o *crtend.o *crti.* *crtn.*) *(.init)
		*crtend.o(.init)
		*crtn.*(.init)

		*(.text)
		*(.fini)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)

		*crtbegin.o(.ctors)
		EXCLUDE_FILE(*crtbegin.o *crtend.o) *(.ctors)
		*crtend.o(.ctors)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
	}
	
	_kernel_end = ALIGN(4K);
}
