#include <dev/pcps2.h>
#include <threads.h>
#include "interrupt.h"
#include "kernel.h"
#include "multiboot.h"
#include "config.h"
#include "pmem.h"
#include "mem.h"
#include "devices.h"
#include "dev/tty.h"
#include "page.h"
#include "threads.h"
#include "sched.h"
#include "cpu.h"
#include "vfs.h"

#include "dev/tmpfs.h"

#if TARGET==i386
#include "arch/i386/dev/vga.h"
#include "arch/i386/dev/pit.h"
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern u8 kernel_end;
extern u8 kernel_start;

#define KERNEL_START &kernel_start
#define KERNEL_END &kernel_end
#define KERNEL_SIZE (size_t)KERNEL_END - (size_t)KERNEL_START

multiboot_info_t* multiboot;
size_t memory_available = 0;
size_t mb_minimum_addr;

unsigned int runtime_us = 0;
unsigned long long runtime_s = 0;

long long kernel_time()
{
	return runtime_s;
}

int kernel_time_us()
{
	return runtime_us;
}

void kernel_time_add(unsigned long long us)
{
	unsigned long long new_us = us + runtime_us;
	runtime_us = new_us;
	runtime_s += new_us / 1000000;
	runtime_us %= 1000000;

}

void kernel_log(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	// We do it like this because kprintf does not support leading zeroes yet.
	int c1 = (runtime_us / 100000) % 10;
	int c2 = (runtime_us / 10000) % 10;
	int c3 = (runtime_us / 1000) % 10;
	int c4 = (runtime_us / 100) % 10;
	int c5 = (runtime_us / 10) % 10;
	int c6 = (runtime_us / 1) % 10;

	kprintf("[%l.%d%d%d%d%d%d] ", runtime_s, c1, c2, c3, c4, c5, c6);
	kvprintf(format, args);
	kputchar('\n');
	va_end(args);
}

void kernel_panic(const char* format, ...)
{
	kernel_log("Panicking!!! (is the pizza gone?)\n");

	va_list args;
	va_start(args, format);
	kvprintf(format, args);
	va_end(args);

	asm volatile ("cli");
	while (1)
		asm volatile ("hlt");
}

/**
 * Initialises the physical memory allocator and adds
 * several mappings to it.
 */
void kernel_init_pmem()
{
	pmem_init(KERNEL_END, memory_available);
	pmem_set(KERNEL_START, KERNEL_SIZE, PMEM_USED);

	// First set everything below 1MB to RESERVED
	pmem_set((void*) NULL, MB(1), PMEM_RESERVED);

	if ((multiboot->flags & MULTIBOOT_INFO_MEM_MAP) == 0)
			kernel_panic("No memory map");

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
 * Initialises the CPU.
 */
void kernel_init_cpu()
{
	cpu_init();
}

/**
 * Initialises paging.
 */
void kernel_init_paging()
{
	page_init();
	page_idmap(&kernel_start, &kernel_end - &kernel_start);
	pmem_register_pages();
	// ID-Map the lowest 1 megabyte
	page_idmap((void*)0, MB(1));
	page_enable();
}

void kernel_loop()
{
	while (1)
	{
		kernel_log("Looped");
		//thread_set_paused(current_thread, true);
		//thread_leave();
	}
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
	UNUSED(magic);
	/*
	 * Information for the multiboot header can be found at:
	 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
	 */
	multiboot = mbd;
	mb_minimum_addr = kernel_end;

	// Detect memory
	if ((mbd->flags & MULTIBOOT_INFO_MEMORY) > 0)
		memory_available = (mbd->mem_lower + mbd->mem_upper) * KB(1);
	else
		kernel_panic("No memory");

	kernel_init_cpu();
	kernel_init_pmem();
	kernel_init_paging();

	//printf("Init interrupts... ");

	mem_init();
	interrupt_init();
	//printf("DONE\n");

	// Enable VGA output if possible
	#ifdef ENABLE_VGA
	vga_init();
	tty_set_tty(device_get_first(VGA));
	#endif
	tty_clear();

	// Show startup screen.
	kernel_log("Starting pizza-os (yum!)...");
	if (memory_available < MB(4))
		kernel_log("Memory detected: %u KiB", memory_available / KB(1));
	else
		kernel_log("Memory detected: %u MiB", memory_available / MB(1));

	kernel_log("Kernel range: 0x%p to 0x%p", KERNEL_START, KERNEL_END);

	/*printf("Init mem... ");
	mem_init();
	printf("DONE\n");*/

	thread_init();


	/*register u32 *ebp asm("esp");
	kprintf("Stack: 0x%X\n", ebp);
	//while (1);

	kprintf("Testing memory...\n");
	void* mem_one = mem_alloc(1);*/
	/*void* mem_two = malloc(8192);
	void* mem_three = mem_alloc(128);*/
	/*kprintf("  ONE 0x%X\n", (size_t)mem_one);
	void* mem_two = malloc(8192);
	kprintf("  TWO 0x%X\n", (size_t)mem_two);
	void* mem_three = mem_alloc(128);
	kprintf("  THR 0x%X\n", (size_t)mem_three);*/

	/*mem_free(mem_two);
	mem_two = mem_alloc(4096);
	void* mem_four = mem_alloc(512);

	printf("  TWO 0x%X\n", mem_two);
	printf(" FOUR 0x%X\n", mem_four);
	while(1);

	printf("THREE 0x%X\n", (size_t)mem_three);*/
	//printf("THREE 0x%X 0x%X 0x%X\n", mem_three, mem_three, mem_three);

	//asm("int $0x20");

	//kprintf("Ok\n");
	pit_init();
	pcps2_init();

	/*
	kprintf("Initialising VFS\n");
#ifdef ENABLE_TMPFS
	tmpfs_init();
#endif
	vfs_init();
	kprintf("Done\n");
	*/

	//while (1);
	//thread_create(kernel_loop);
	sched_main();
	while (1);

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
	kprintf("Hello world!\n");
	kprintf("Percent test %%\n");
	kprintf("Decimal test %d\n", 1385ul);
	kprintf("Hexadecimal test 0x%x\n", 0xc0ffee);
	kprintf("Hexadecimal test 0x%X\n", 0xc0ffee);

	kprintf("Reached end of kernel_main()\n");
}




















