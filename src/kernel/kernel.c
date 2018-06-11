#include "threads.h"
#include "interrupt.h"
#include "kernel.h"

#include "api/keyboard.h"
#include "multiboot.h"
#include "config.h"
#include "pmem.h"
#include "mem.h"
#include "page.h"
#include "threads.h"
#include "sched.h"
#include "cpu.h"
#include "vfs.h"
#include "fstypes.h"

#include "thread/mutex.h"

#include "devices.h"
#include "dev/pcps2.h"
#include "dev/pckbd.h"
#include "dev/tty.h"
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

	thread_init();

	vfs_init();
	tmpfs_init();
	//filesystem_t* tmpfs = tmpfs_init();
	//vfs_mount("/", tmpfs);
	vfs_mount("/", "tmpfs", "tmpfs", 0, NULL);

	vfs_mkdir("/dev");
	vfs_mkdir("/dirB");
	vfs_mkdir("/dirB/subDirA");
	vfs_mkdir("/dirB/subDirB");

	kernel_log("Writing file");
	FILE file = vfs_open_file("/file", O_WRITE | O_CREATE | O_READ);
	size_t amount = vfs_write_file(file, "Hello, world!", 14);
	kernel_log("Wrote %d bytes", amount);
	vfs_close_file(file);
	kernel_log("Done. Reading it");

	file = vfs_open_file("/file", O_READ);
	char buf[32];
	amount = vfs_read_file(file, buf, 32);
	kernel_log("Read %d bytes", amount);
	vfs_close_file(file);

	kernel_log("Content: '%s'", buf);
	vfs_rm("/file");
	test_vfs();


	/*kernel_log("Listing filesystem drivers");
	bus_it it;
	module_it_begin(&it, FILESYSTEM);
	filesystem_mounter_t* fs;
	while ((fs = module_it_next(&it)) != NULL)
	{
		kernel_log("Found %s", fs->name);
	}
	kernel_log("Done");*/


	keyboard_init();


	pit_init();

	#ifdef ENABLE_PS2
	pcps2_init();
	#endif
	#ifdef ENABLE_PS2KBD
	pckbd_init(device_get_first(PS2));
	#endif

	kernel_log("Listing root");
	list_dir("/");
	kernel_log("Finished");

	/*FILE f = vfs_open_file("/dev/kbd", O_READ);
	char kbuf[16];
	size_t read = vfs_read_file(f, kbuf, 16);
	kernel_log("Read %d", read);
	kernel_log("%s", buf);
	vfs_close_file(f);*/

	sched_main();

	kernel_panic("Reached end of kernel_main()\n");
}




















