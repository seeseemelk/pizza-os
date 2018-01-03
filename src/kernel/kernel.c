#include "kernel.h"
#include "multiboot.h"
#include "config.h"
#include "mem.h"
#include "dev/devices.h"
#include "dev/tty.h"

#if TARGET==PC
#include "arch/pc/cpu.h"
#include "arch/pc/page.h"
#include "arch/pc/dev/vga.h"
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

size_t memory_available = 0;

void kernel_panic(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	printf("Panicking!!! (is the pizza gone?)\n");
	vprintf(format, args);
	putchar('\n');

	va_end(args);

	while (1);
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{

	// Detect memory
	if ((mbd->flags & MULTIBOOT_INFO_MEMORY) > 0)
		memory_available = mbd->mem_lower + mbd->mem_upper;

	// Init memory allocator
	mem_init(MEMORY_LOCATION, memory_available - MEMORY_LOCATION);

	// Init paging
	page_init();

	// Make sure that the memory allocator added it's page entries
	mem_paging_enabled();

	// Enable early VGA output if possible
#ifdef ENABLE_VGA
	tty_set_tty(vga_init());
#endif

	// Clean up the screen
	tty_clear();
	tty_print("Starting pizza-os (yum!)...\n");
	if (memory_available < 4096)
		printf("Memory detected: %u KiB\n", memory_available);
	else
		printf("Memory detected: %u MiB\n", memory_available/1024);

	// Initialise the memory allocator
	/*mem_init(KERNEL_LOCATION + MB(16), MB(10));
	void* test_ptr = mem_alloc(1);
	printf("Allocated test_ptr at 0x%p\n", test_ptr);
	void* test_ptr2 = mem_alloc(1);
	printf("Allocated test_ptr2 at 0x%p\n", test_ptr2);
	mem_free(test_ptr);
	printf("Freed test_ptr\n");
	void* test_ptr3 = mem_alloc(10);
	printf("Allocated test_ptr3 at 0x%p\n", test_ptr3);
	void* test_ptr4 = mem_alloc(512);
	printf("Allocated test_ptr4 at 0x%p\n", test_ptr4);
	mem_free(test_ptr);
	printf("Freed test_ptr\n");
	void* test_ptr5 = mem_alloc(8192);
	printf("Allocated test_ptr5 at 0x%p\n", test_ptr5);

	printf("Eating all the memory\n");
	while (1)
	{
		mem_alloc(4096);
	}*/

	// Initialise the CPU
	cpu_init();

	// Initialise paging
	page_init();

	// Do other stuff
	printf("Hello world!\n");
	printf("Percent test %%\n");
	printf("Decimal test %d\n", 1385ul);
	printf("Hexadecimal test 0x%x\n", 0xc0ffee);
	printf("Hexadecimal test 0x%X\n", 0xc0ffee);

	printf("Reached end of kernel_main()\n");
}




















