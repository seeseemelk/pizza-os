#include "kernel.h"
#include "multiboot.h"
#include "config.h"
#include "pmem.h"
#include "dev/devices.h"
#include "dev/tty.h"
#include "page.h"

#if TARGET==PC
#include "arch/pc/cpu.h"
#include "arch/pc/dev/vga.h"
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

extern u8 kernel_end;
extern u8 kernel_start;
extern u8 stack_top;
extern u8 stack_bottom;

#define KERNEL_START &kernel_start
#define KERNEL_END &kernel_end
#define KERNEL_SIZE (size_t)KERNEL_END - (size_t)KERNEL_START

multiboot_info_t* multiboot;
size_t memory_available = 0;
size_t mb_minimum_addr;

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

/**
 * Rounds a number upwards to 4 KiB.
 */
/*unsigned int ceil4kb(unsigned int n)
{
	return (((n-1) / KB(4)) + 1) * KB(4);
}

unsigned long long lceil4kb(unsigned long long n)
{
	return (((n-1) / KB(4)) + 1) * KB(4);
}
*/
/**
 * Initialise the memory map so that each entry marked free starts on a 4 KiB boundary.
 */
/*void kernel_init_mmap()
{
	multiboot_memory_map_t* map = (multiboot_memory_map_t*) multiboot->mmap_addr;
	multiboot_memory_map_t* last_map = (multiboot_memory_map_t*) (multiboot->mmap_addr + multiboot->mmap_length);

	while (map < last_map)
	{
		if (map->addr < mb_minimum_addr && map->addr + map->len >= mb_minimum_addr)
			printf("Potential memory that might get missed\n");

		if (map->type == MULTIBOOT_MEMORY_AVAILABLE)
		{
			if (map->len < MB(4))
				printf("Free memory at 0x%p and a size of %d KiB\n", (u32)map->addr, (u32) map->len / KB(1), map->size);
			else
				printf("Free memory at 0x%p and a size of %d MiB\n", (u32)map->addr, (u32) map->len / MB(1), map->size);
			// Set the new address
			multiboot_uint64_t ori = map->addr;
			map->addr = ceil4kb(map->addr);
			if (map->addr != ori)
				printf("Moved to 0x%p\n", map->addr);
			// And reduce the size accordingly
			map->len -= (map->addr - ori);
		}

		map = (multiboot_memory_map_t*) ((size_t)map + map->size + 4);
	}
}*/

void kernel_init_pmem()
{
	pmem_init(KERNEL_END, memory_available);
	pmem_set(KERNEL_START, KERNEL_SIZE, PMEM_USED);

	// First set everything below 1MB to RESERVED
	pmem_set((void*) NULL, MB(1), PMEM_RESERVED);

	multiboot_memory_map_t* map = (multiboot_memory_map_t*) multiboot->mmap_addr;
	multiboot_memory_map_t* last_map = (multiboot_memory_map_t*) (multiboot->mmap_addr + multiboot->mmap_length);
	while (map < last_map)
	{
		if (map->type != MULTIBOOT_MEMORY_AVAILABLE)
			pmem_set((void*) (size_t) map->addr, map->len, PMEM_RESERVED);

		map = (multiboot_memory_map_t*) ((size_t)map + map->size + 4);
	}
}

/**
 * Finds the next free memory slot in the memory map given by the multiboot header.
 * Note that all the memory address that are returned by the function are physical
 * address. They should still be translated to take paging into account.
 */
/*void* kernel_find_next_free_mem(size_t amount)
{
	multiboot_memory_map_t* map = (multiboot_memory_map_t*) multiboot->mmap_addr;
	multiboot_memory_map_t* last_map = (multiboot_memory_map_t*) (multiboot->mmap_addr + multiboot->mmap_length);
	amount = ceil4kb(amount);

	while (map < last_map)
	{
		if (map->type == MULTIBOOT_MEMORY_AVAILABLE && map->addr >= mb_minimum_addr)
		{
			if (map->len == amount) // If we use all of the space, set it as unusable
			{
				map->type = -1;
				return (void*) (size_t) map->addr;
			}
			else if (map->len > amount) // If there is still space left, just increase the pointer over the now-used space
			{
				map->addr += amount;
				return (void*) (size_t)map->addr - amount;
			}
		}

		map = (multiboot_memory_map_t*) ((size_t)map + map->size + 4);
	}
	return NULL;
}*/

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
	/*
	 * Information for the multiboot header can be found at:
	 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
	 */
	multiboot = mbd;
	mb_minimum_addr = kernel_end;

	// Enable early VGA output if possible
#ifdef ENABLE_VGA
	device* vga_dev = vga_init();
	tty_set_tty(vga_dev);
#endif

	// Clean up the screen
	tty_clear();

	printf("Starting pizza-os (yum!)...\n");
	// Detect memory
	if ((mbd->flags & MULTIBOOT_INFO_MEMORY) > 0)
	{
		memory_available = (mbd->mem_lower + mbd->mem_upper) * KB(1);
		if (memory_available < MB(4))
			printf("Memory detected: %u KiB\n", memory_available / KB(1));
		else
			printf("Memory detected: %u MiB\n", memory_available / MB(1));
	}
	else
		kernel_panic("No memory");

	printf("Kernel range: 0x%p to 0x%p\n", KERNEL_START, KERNEL_END);

	// We need a memory map if we want to correctly do things.
	if ((mbd->flags & MULTIBOOT_INFO_MEM_MAP) == 0)
		kernel_panic("No memory map");

	// Setup the physical memory allocator
	printf("Init PMEM... ");
	kernel_init_pmem();
	printf("DONE\n");

	// Initialise the CPU
	printf("Init CPU... ");
	cpu_init();
	printf("DONE\n");

	// Initialise the multiboot memory map so that
	// each entry starts on a 4 KiB boundary.
	//kernel_init_mmap();

	// Initialise the pager.
	printf("Init page... ");
	page_init();
	printf("DONE\n");

	// ID Map the kernel
	printf("ID Map kernel... ");
	page_idmap(&kernel_start, &kernel_end - &kernel_start);
	printf("DONE\n");

	printf("Paging device mappings... ");
	device_invoke(vga_dev, PAGING_ENABLED);

	printf("Register pmem pages... ");
	pmem_register_pages();
	printf("DONE\n");


	register int *stack asm("esp");
	printf("Stack = 0x%p\n", stack);

	/*printf("Registering stack... ");
	size_t stack_size = (size_t)&stack_top - (size_t)&stack_bottom;
	page_idmap(&stack_bottom, stack_size);
	printf("DONE\n");*/

	// Enable paging
	printf("Enabling paging... ");
	page_enable();
	printf("DONE\n");

	page_map((void*)0x00C00000, (void*)0x00B00000, KB(16));
	printf("Ok");
	while (1);


	// ID-map the kernel.
	if ((mbd->flags & MULTIBOOT_INFO_ELF_SHDR) == 0)
				kernel_panic("Cannot ID-map the kernel");
	multiboot_elf_section_header_table_t* mbd_elf = &mbd->u.elf_sec;
	printf("MBD Addr: 0x%p\n", mbd_elf->addr);
	printf("MBD Size: %d\n", mbd_elf->size);
	printf("MBD Num : %d\n", mbd_elf->num);
	//page_idmap(mbd_elf->)


	// Init memory allocator
	//mem_init(MEMORY_LOCATION, memory_available - MEMORY_LOCATION);

	// Init paging
	//page_init();
	//while(1) ;

	// Make sure that the memory allocator added it's page entries
	//mem_paging_enabled();

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
	//page_init();

	// Do other stuff
	printf("Hello world!\n");
	printf("Percent test %%\n");
	printf("Decimal test %d\n", 1385ul);
	printf("Hexadecimal test 0x%x\n", 0xc0ffee);
	printf("Hexadecimal test 0x%X\n", 0xc0ffee);

	printf("Reached end of kernel_main()\n");
}




















