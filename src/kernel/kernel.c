#include "threads.h"
#include "interrupt.h"
#include "kernel.h"

#include "api/keyboard.h"
#include "multiboot.h"
#include "config.h"
#include "pmem.h"
#include "mem.h"
#include "threads.h"
#include "sched.h"
#include "cpu.h"
#include "vfs.h"
#include "fstypes.h"
#include "gdbstub.h"

#include "thread/mutex.h"

#include "devices.h"
#include "dev/eserial.h"
#include "dev/pcps2.h"
#include "dev/pckbd.h"
#include "dev/tty.h"
#include "dev/tmpfs.h"
#include "dev/null.h"
#include "dev/zero.h"
#include "paging.h"

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
	{
		/* The bootloader did not provide us with a memory map.
		 * Lets use everything about 1MB as free memory.
		 */
		kernel_log("No memory map. Using 0x00000000 to 0x00100000 as RESERVED");
		pmem_set(0, MB(1), PMEM_RESERVED);
	}
	else
	{
		/* The bootloader provided us with a memory map that we can use. */
		multiboot_memory_map_t* map = (multiboot_memory_map_t*) multiboot->mmap_addr;
		multiboot_memory_map_t* last_map = (multiboot_memory_map_t*) (multiboot->mmap_addr + multiboot->mmap_length);
		while (map < last_map)
		{
			if (map->type != MULTIBOOT_MEMORY_AVAILABLE)
				pmem_set((void*) (size_t) map->addr, map->len, PMEM_RESERVED);

			map = (multiboot_memory_map_t*) ((size_t)map + map->size + 4);
		}
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
	/*page_idmap(&kernel_start, &kernel_end - &kernel_start);
	pmem_register_pages();
	// ID-Map the lowest 1 megabyte
	page_idmap((void*)0, MB(1));
	page_enable();*/
}

void list_dir(const char* path)
{
	DIR dir = vfs_open_dir(path);
	dirent_t dirent;
	while (vfs_next_dir(dir, &dirent))
	{
		size_t path_length = strlen(path);
		size_t filelength = strlen(dirent.name);
		char* cat = malloc(path_length + filelength + 1);
		strcpy(cat, path);
		if (path_length > 1)
			cat[path_length++] = '/';
		strcpy(cat+path_length, dirent.name);
		if (dirent.type == FDIR)
		{
			kernel_log("D %s", cat);
			list_dir(cat);
			free(cat);
		}
		else
			kernel_log("F %s", cat);
	}
	vfs_close_dir(dir);
}

void kernel_test()
{
	char kbuf[16];
	FILE file = vfs_open_file("/dev/kbd", O_READ);
	while (1)
	{
		vfs_read_file(file, kbuf, 1);
		kernel_log("Pressed %c", kbuf[0]);
	}
	vfs_close_file(file);
}

int kernel_cmdline_next(char* cmd, int* start, int* end)
{
	if (cmd[*end] == '\0')
		return 0;

	*start = *end;
	if (cmd[*start] == ' ' || cmd[*start] == ',')
		(*start)++;

	*end = *start;
	while (cmd[*end] != ' ' && cmd[*end] != ',' && cmd[*end] != '\0')
	{
		(*end)++;
	}
	return *end - *start;
}

void kernel_init_gdb()
{
	set_debug_traps();
	kernel_log("Waiting for GDB...");
	__asm__("int3");
	kernel_log("GDB Connected!");
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
	bool enable_gdb = false;
	eserial_init();

	kernel_log("Argument list: '%s'", mbd->cmdline);
	int start = 0, end = 0;
	int size;
	while ((size = kernel_cmdline_next((char*) mbd->cmdline, &start, &end)) > 0)
	{
		kernel_log("Size: %d", size);
		char str[size];
		strncpy(str, (char*) mbd->cmdline + start, size);
		kernel_log("Argument: '%s'", str);
		if (strcmp(str, "gdb") == 0)
			enable_gdb = true;
	}
	kernel_log("End of argument list");

	kernel_log("Enter kernel_main");
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

	kernel_log("Init cpu");
	kernel_init_cpu();
	kernel_log("Done");
	kernel_log("Init pmem");
	kernel_init_pmem();
	kernel_log("Done");
	kernel_log("Init paging");
	kernel_init_paging();
	kernel_log("Done");

	//printf("Init interrupts... ");

	kernel_log("Init mem");
	mem_init();
	kernel_log("Done");
	kernel_log("Init interrupts");
	interrupt_init();
	kernel_log("Done");
	//printf("DONE\n");


	// Enable VGA output if possible
	#ifdef ENABLE_VGA
	vga_init();
	tty_set_tty(device_get_first(VGA));
	#endif
	tty_clear();

	if (enable_gdb)
		kernel_init_gdb();

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
	kernel_log("Mounting root");
	vfs_mount("/", "tmpfs", "tmpfs", 0, NULL);
	kernel_log("Creating /dev");
	vfs_mkdir("/dev");
	kernel_log("Mounting /dev");
	vfs_mount("/dev", "tmpfs", "tmpfs", 0, NULL);

	kernel_log("Continuing");
	vfs_mkdir("/dirB");
	vfs_mkdir("/dirB/subDirA");
	vfs_mkdir("/dirB/subDirB");

	kernel_log("Writing file");
	FILE file = vfs_open_file("/dev/file", O_WRITE | O_CREATE | O_READ);
	size_t amount = vfs_write_file(file, "Hello, world!", 14);
	kernel_log("Wrote %d bytes", amount);
	vfs_close_file(file);
	kernel_log("Done. Reading it");

	file = vfs_open_file("/dev/file", O_READ);
	char buf[32];
	amount = vfs_read_file(file, buf, 32);
	kernel_log("Read %d bytes", amount);
	vfs_close_file(file);

	kernel_log("Content: '%s'", buf);
	//vfs_rm("/dev/file");


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
	null_init();
	zero_init();

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

	thread_create(kernel_test);

	sched_main();

	kernel_panic("Reached end of kernel_main()\n");
}




















