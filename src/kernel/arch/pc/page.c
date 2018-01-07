/*
 * page.c
 *
 *  Created on: Jan 7, 2018
 *      Author: seeseemelk
 */
#include "arch/pc/page.h"
#include "arch/pc/asmfn.h"
#include "pmem.h"
#include "kernel.h"
#include "cdefs.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

page_entry* page_directory;
page_entry* page_metatable;

page_entry* mem_window_entry;
void* mem_window;

bool paging_enabled = false;

void* page_get_address(page_entry* entry)
{
	return (void*) ((*entry) & 0xFFFFF000);
}

void page_set_address(page_entry* entry, void* address)
{
	(*entry) = ((*entry) & 0xFFF) | ((size_t)address & 0xFFFFF000);
}

void page_set_flags(page_entry* entry, page_flags flags)
{
	(*entry) = (size_t)page_get_address(entry) | flags;
}

page_flags page_get_flags(page_entry* entry)
{
	return (*entry) & 0xFFF;
}

bool page_is_present(page_entry* entry)
{
	return (page_get_flags(entry) & PAGE_PRESENT) != 0;
}

/**
 * Change the memory window so that a window to 4 KB of memory can be accessed.
 */
void mem_window_set(void* phys)
{
	if (!paging_enabled)
	{
		// Paging is disabled so we can
		// just reference the address.
		mem_window = phys;
	}
	else
	{
		// Paging is enabled, but we have
		// got a pointer to the memory window entry.
		// We just move this to point to our new address.
		page_set_address(mem_window_entry, phys);
	}
}

/**
 * Adds a page table to the metatable.
 * The physical address of the metatable should be given.
 * It will finally return an address that can be used to access the page.
 * Note that this pointer is only valid until paging is toggled on or off.
 */
page_entry* page_metatable_add(page_entry* tbl_phys)
{
	for (size_t tbl_i = 1; tbl_i < 1024; tbl_i++)
	{
		page_entry* mtbl_entry = page_metatable + tbl_i;
		if (!page_is_present(mtbl_entry))
		{
			page_set_address(mtbl_entry, tbl_phys);
			page_set_flags(mtbl_entry, PAGE_PRESENT | PAGE_RW);
			// When we're not in paging, page_metatable is a physical pointer,
			// But when it gets enabled it is converted into a virtual pointer.
			return page_metatable + tbl_i * KB(4);
		}
	}
	kernel_panic("No more pages free");
	return NULL;
}

/**
 * Create a table in the directory entry.
 * Returns a pointer to the first element of the newly created table.
 * Note that this pointer is only valid for until a next call to any paging functions
 * Note that this pointer is only valid until paging is toggled on or off.
 */
page_entry* page_create_table(page_entry* dir_entry)
{
	page_entry* new_tbl = pmem_alloc(KB(4));
	mem_window_set(new_tbl);
	memset(mem_window, 0, KB(4));
	page_set_address(dir_entry, new_tbl);
	return page_metatable_add(new_tbl);
}

/**
 * Follows an entry.
 * It returns a pointer to a memory address that can be used to manipulate
 * the content to which the entry points.
 * Note that this pointer is only valid for until a next call to any paging functions
 * (except for page_get_* and page_set_* functions)
 */
void* page_follow_entry(page_entry* entry)
{
	void* addr = page_get_address(entry);
	if (!paging_enabled)
		return addr;
	else
	{
		mem_window_set(entry);
		return mem_window;
	}
}

/**
 * Allocates a single page.
 */
void page_alloc_page(void* virt, void* phys)
{
	// We calculate the index into both the page directory and the page table.
	size_t dir_i = (size_t)virt / MB(4);
	size_t tbl_i = (size_t)virt % MB(4) / KB(4);

	// We then get a usable reference to the table that contains the page.
	page_entry* dir_entry = page_directory + dir_i;
	page_entry* tbl;
	if (!page_is_present(dir_entry))
		tbl = page_create_table(dir_entry);
	else
		tbl = page_follow_entry(dir_entry);

	// And we add the entry to it.
	page_entry* tbl_entry = tbl + tbl_i;
	page_set_address(tbl_entry, phys);
	page_set_flags(tbl_entry, PAGE_PRESENT | PAGE_RW);
}

void page_map(void* virt, void* phys, size_t length)
{
	// We need to calculate how many blocks we need to allocate.
	size_t blocks = length / KB(4);

	// And we then allocate them using page_alloc_block()
	for (size_t block = 0; block < blocks; block++)
	{
		void* virt_adj = (void*) ((size_t) virt + block * KB(4));
		void* phys_adj = (void*) ((size_t) virt + block * KB(4));
		page_alloc_page(virt_adj, phys_adj);
	}
}

void page_idmap(void* virt, size_t length)
{
	page_map(virt, virt, length);
}

void page_init()
{
	// First we allocate the page directory and the metatable.
	page_directory = pmem_alloc(KB(4));
	page_metatable = pmem_alloc(KB(4));

	// And we clear it.
	memset(page_directory, 0, KB(4));
	memset(page_metatable, 0, KB(4));

	// Then we add the metatable to itself as the very first entry
	page_set_address(page_metatable, page_metatable);
	page_set_flags(page_metatable, PAGE_PRESENT | PAGE_RW);

	// We must then add the metatable to the page directory.
	// We are going to add it to the very last entry, which
	// also corresponds to the very last 4 MB of the 4 GB virtual
	// address space.
	page_entry* dir_entry = page_directory + 1023;
	page_set_address(dir_entry, page_metatable);
	page_set_flags(dir_entry, PAGE_PRESENT | PAGE_RW);

	// Finally we ID-map the page directory.
	page_idmap(page_directory, KB(4));
}

void page_enable()
{
	printf("Enabling paging\n");

	// Load the address of the page directory into CR3
	asm_load_cr3_page_dir((size_t) page_directory);
	printf("Set page");
	while (1);

	// Set CR0.PG to 1.
	// This will put the CPU in 32-bit paging mode.
	// (see Intel Manual Volume 3, 4.1.2)
	asm_enable_cr0_pepg();

	printf("Paging enabled\n");
}















