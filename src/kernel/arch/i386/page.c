/*
 * page.c
 *
 *  Created on: Jan 7, 2018
 *      Author: seeseemelk
 */
#include "../i386/page.h"

#include "pmem.h"
#include "kernel.h"
#include "cdefs.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../i386/asmfn.h"

page_entry* page_directory;
page_entry* page_metatable;

page_entry* mem_window_entry;
volatile void* mem_window;

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
		asm_invlpg(mem_window);
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
			return (page_entry*)((void*)page_metatable + tbl_i * KB(4));
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
	if (new_tbl == NULL)
		kernel_panic("Could not allocate new page table");

	if (paging_enabled)
	{
		mem_window_set(new_tbl);
		memset((void*)mem_window, 0, KB(4));
	}
	else
	{
		memset(new_tbl, 0, KB(4));
	}
	page_set_address(dir_entry, new_tbl);
	page_entry* tbl_access = page_metatable_add(new_tbl);
	page_set_flags(dir_entry, PAGE_PRESENT | PAGE_RW);
	return tbl_access;
}

/**
 * Follows an entry.
 * It returns a pointer to a memory address that can be used to manipulate
 * the content to which the entry points.
 * Note that this pointer is only valid for until a next call to any paging functions
 * (except for page_get_* and page_set_* functions)
 */
volatile void* page_follow_entry(page_entry* entry)
{
	void* addr = page_get_address(entry);
	if (!paging_enabled)
		return addr;
	else
	{
		mem_window_set(addr);
		return mem_window;
	}
}

/**
 * Gets a pointer to the page table that manages a specified section of the virtual memory.
 * This function only works when paging has been enabled.
 */
page_entry* page_get_table(void* virt)
{
	size_t dir_i = (size_t)virt / MB(4);
	page_entry* dir_entry = page_directory + dir_i;
	void* tbl_addr = page_get_address(dir_entry);

	for (size_t mtbl_i = 1; mtbl_i < 1024; mtbl_i++)
	{
		page_entry* mtbl_entry = page_metatable + mtbl_i;
		if (page_is_present(mtbl_entry) && (page_get_address(mtbl_entry) == tbl_addr))
		{
			return (page_entry*) (1023 * MB(4) + mtbl_i * KB(4));
		}
	}
	while (1);
	kernel_panic("Could not find table");
	return NULL;
}

/**
 * Tries to find some free virtual memory.
 */
void* page_find_free(size_t pages_needed)
{
	void* found_addr;
	size_t found_amount = 0;

	for (size_t dir_i = 0; dir_i < 1023; dir_i++)
	{
		page_entry* dir_entry = page_directory + dir_i;
		if (!page_is_present(dir_entry))
		{
			if (found_amount == 0)
				found_addr = (void*) (dir_i * MB(4));
			found_amount += 1024;
			if (found_amount >= pages_needed)
				goto found_enough;
		}
		else
		{
			// We found an existing directory entry,
			// lets browse it.
			page_entry* tbl = page_get_table((void*) (dir_i * MB(4)));
			for (size_t tbl_i = 0; tbl_i < 1024; tbl_i++)
			{
				page_entry* tbl_entry = tbl + tbl_i;
				if (!page_is_present(tbl_entry))
				{
					if (found_amount == 0)
						found_addr = (void*) (dir_i * MB(4) + tbl_i * KB(4));
					found_amount++;
					if (found_amount >= pages_needed)
						goto found_enough;
				}
				else
				{
					found_amount = 0;
				}
			}
		}
	}

	found_enough:
	if (found_amount < pages_needed)
	{
		while(1);
	}
	return found_addr;
}

/**
 * Allocates a single page.
 */
void page_alloc_page(void* virt, void* phys)
{
	// We calculate the index into both the page directory and the page table.
	size_t dir_i = (size_t)virt / MB(4);
	size_t tbl_i = ((size_t)virt % MB(4)) / KB(4);

	// We then get a usable reference to the table that contains the page.
	page_entry* dir_entry = page_directory + dir_i;
	page_entry* tbl;
	if (!page_is_present(dir_entry))
	{
		tbl = page_create_table(dir_entry);
	}
	else
		tbl = (page_entry*) page_follow_entry(dir_entry);

	// And we add the entry to it.
	// Note that if the page is already present,
	// we should invalidate the page after loading it.
	page_entry* tbl_entry = tbl + tbl_i;
	page_set_address(tbl_entry, phys);
	const bool was_present = page_is_present(tbl_entry);
	page_set_flags(tbl_entry, PAGE_PRESENT | PAGE_RW);
	if (was_present)
		asm_invlpg(virt);
}

void* page_alloc_phys(void* phys, size_t length)
{
	size_t pages = ceildiv(length, KB(4));
	void* virt = page_find_free(pages);
	for (size_t page = 0; page < pages; page++)
	{
		void* virt_adj = (void*) (virt + page * KB(4));
		void* phys_adj = (void*) (phys + page * KB(4));
		page_alloc_page(virt_adj, phys_adj);
	}
	return virt;
}

void* page_alloc(size_t length)
{
	size_t pages = ceildiv(length, KB(4));
	void* virt = page_find_free(pages);
	for (size_t page = 0; page < pages; page++)
	{
		void* virt_adj = (void*) (virt + page * KB(4));
		void* phys_adj = pmem_alloc(KB(4));
		page_alloc_page(virt_adj, phys_adj);
	}
	return virt;
}

void page_map(void* virt, void* phys, size_t length)
{
	if ((size_t)virt % KB(4) != 0)
		kernel_panic("Virtual address not on page boundary");
	if ((size_t)phys % KB(4) != 0)
		kernel_panic("Physical address not on page boundary");
	if (length % KB(4) != 0)
		kernel_panic("Block length not on page boundary");

	// We need to calculate how many blocks we need to allocate.
	size_t blocks = length / KB(4);

	// And we then allocate them using page_alloc_block()
	for (size_t block = 0; block < blocks; block++)
	{
		void* virt_adj = (void*) ((size_t) virt + block * KB(4));
		void* phys_adj = (void*) ((size_t) phys + block * KB(4));
		page_alloc_page(virt_adj, phys_adj);
	}
}

void page_idmap(void* virt, size_t length)
{
	page_map(virt, virt, length);
}

/**
 * Frees a single page.
 */
void page_free_page(void* virt)
{
	size_t tbl_i = (size_t)virt % MB(4) / KB(4);
	page_entry* tbl = page_get_table(virt);
	page_entry* tbl_entry = tbl + tbl_i;
	page_set_flags(tbl_entry, 0);
	asm_invlpg(virt);
}

void page_free(void* virt, size_t length)
{
	size_t pages = length / KB(4);
	for (size_t page = 0; page < pages; page++)
		page_free_page(virt + page * KB(4));
}

void page_init()
{
	// First we allocate the page directory and the metatable.
	page_directory = pmem_alloc(KB(4));
	if (page_directory == NULL)
		kernel_panic("Could not allocate page directory");
	page_metatable = pmem_alloc(KB(4));
	if (page_metatable == NULL)
		kernel_panic("Could not allocate metatable");

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
	// Before we turn on paging we should allocate a page
	// for the memory window. We are going to allocate it
	// right before the metatable.
	page_alloc_page((void*) (1023 * MB(4) - KB(4)), (void*) mem_window);

	// Set this variable to true
	paging_enabled = true;

	// Also allocate some physical memory for the memory window.
	//page_idmap(mem_window, KB(4));

	// Load the address of the page directory into CR3
	asm_load_cr3_page_dir((size_t) page_directory);

	// Set CR0.PG to 1.
	// This will put the CPU in 32-bit paging mode.
	// (see Intel Manual Volume 3, 4.1.2)
	asm_enable_cr0_pepg();

	// Get the virtual address to the metatable.
	page_metatable = (page_entry*) (1023 * MB(4));

	// Now we still need to get the handle for the memory window.
	page_entry* tbl = page_get_table((void*) (1022 * MB(4)));
	mem_window_entry = (page_entry*)(tbl + 1023);
	mem_window = (void*) (1023 * MB(4) - KB(4));

	/*printf("TBL = 0x%X\n", tbl);
	printf("Memory window = 0x%X\n", (size_t)mem_window);
	printf("Memory window entry = 0x%X\n", *mem_window_entry);
	//while(1);*/

	/*// Find the entry that points to the memory window.
	for (size_t block = 1; block < 1024; block++)
	{
		page_entry* entry = page_metatable + block;
		if (page_is_present(entry))
			printf("Entry %d = 0x%X\n", block, (size_t)entry);
		if (page_is_present(entry) && page_get_address(entry) == mem_window)
		{
			mem_window_entry = entry;
			break;
		}
	}*/

	/*printf("Mem_window_entry = 0x%p\n", (size_t)mem_window_entry);
	mem_window_set((void*) 0x1000);*/
	//while(1) ;
}















