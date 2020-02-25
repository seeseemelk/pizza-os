ENTRY(_start)

SECTIONS
{
	. = 4K;

	.text ALIGN(4K):
	{	
		*crti.*(.init)
		*crtbegin.o(.init)
		EXCLUDE_FILE (*crtbegin.o *crtend.o *crti.* *crtn.o) *(.init)
		*crtend.o(.init)
		*crtn.*(.init)

		*(.text)
		*(.fini)
	}

	.data ALIGN(4K):
	{
		*(.data)
	}

	.bss ALIGN(4K):
	{
		*(COMMON)
		*(.bss)
	}

	.rodata ALIGN(4K):
	{
		*(.rodata)

		*crtbegin.o(.ctors)
		EXCLUDE_FILE(*crtbegin.o *crtend.o) *(.ctors)
		*ctrend.o(.ctors)

		*crtbegin.o(.dtors)
		EXCLUDE_FILE(*crtbegin.o *crtend.o) *(.dtors)
		*ctrend.o(.dtors)
	}
}
