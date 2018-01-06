/*
 * page.c
 *
 *  Created on: Jan 5, 2018
 *      Author: seeseemelk
 */
#include "arch/pc/page.h"
#include "arch/pc/asmfn.h"
#include "pmem.h"
#include "kernel.h"
#include "cdefs.h"

#include <string.h>
#include <stdio.h>

#define MINIMUM_SIZE KB(4*3)

// Contains the page directory.
page_entry* page_directory;

// Contains the page table containing all the page tables.
// It is important to note that in physical memory this table
// comes 4 KiB after the page directory, but in virtual memory
// it comes 4 MiB after the page directory.
page_entry* page_tbl;
void* page_tbl_offset;

size_t page_get_address(page_entry* entry)
{
	return (*entry) & 0xFFFFF000;
}

void page_set_address(page_entry* entry, void* address)
{
	(*entry) = ((*entry) & 0xFFF) | (((size_t) address) & 0xFFFFF000);
}

void page_set_flags(page_entry* entry, page_flags flags)
{
	(*entry) = page_get_address(entry) | flags;
}

page_flags page_get_flags(page_entry* entry)
{
	return (*entry) & 0xFFF;
}

/**
 * Creates a page table and makes sure the dir_entry points towards it.
 */
static page_entry* page_create_table(page_entry* dir_entry)
{
	page_entry* tbl = (page_entry*) pmem_alloc(KB(4));
	memset(tbl, 0, KB(4));

	void* virt;
	for (size_t tbl_i = 0; tbl_i < 1024; tbl_i++)
	{
		page_entry* ptbl_entry = (page_entry*) page_tbl[tbl_i];
		if ((page_get_flags(ptbl_entry) & PAGE_PRESENT) == 0)
		{
			page_set_address(ptbl_entry, tbl);
			page_set_flags(ptbl_entry, PAGE_PRESENT | PAGE_RW);
			virt = (void*) (tbl_i * KB(4) + page_tbl_offset);
			break;
		}
	}

	page_set_address(dir_entry, virt);
	return tbl;
}

/**
 * Gets a pointer to the directory entry that contains a certain virtual address.
 */
static page_entry* page_get_dir_entry(void* virt_addr)
{
	return &page_directory[(size_t)virt_addr / MB(4)];
}

/**
 * Gets a pointer to the table entry that contains a certain virtual address.
 */
static page_entry* page_get_tbl_entry(page_entry* tbl, void* virt_addr)
{
	return &tbl[(size_t)virt_addr % MB(4)];
}

/**
 * Gets a pointer to the table entry that contains a virtual address.
 * If the table does not yet exist, it will be created.
 */
static page_entry* page_follow_dir_entry(page_entry* dir_entry)
{
	return (page_entry*) page_get_address(dir_entry);
}

static page_entry* page_get_entry(void* virt_addr)
{
	page_entry* dir_entry = page_get_dir_entry(virt_addr);
	page_entry* tbl;
	if ((page_get_flags(dir_entry) & PAGE_PRESENT) == 0)
		tbl = page_create_table(dir_entry);
	else
		tbl = page_follow_dir_entry(dir_entry);

	return page_get_tbl_entry(tbl, virt_addr);
}

/**
 * Get the page table the directory entry points towards.
 */
/**
 * Gets the page_entry that describes a given virtual address.
 */
/*static page_entry* page_from_virt(void* virt_addr)
{
	page_entry* dir_entry = page_get_dir_entry(((size_t) virt_addr) / MB(4));
	page_entry* tbl = page_follow_dir_entry(dir_entry);
	return &tbl[(size_t)virt_addr % MB(4)];
}*/

static void page_register_blocks(void* virt_first, size_t blocks)
{
	const void* virt_end = virt_first + blocks * KB(4);
	for (void* i = virt_first; i < virt_end; i += 4096)
	{
		page_entry* entry = page_get_entry(i);
		void* mem = pmem_alloc(KB(4));
		page_set_address(entry, mem);
		page_set_flags(entry, PAGE_PRESENT | PAGE_RW);
	}
}

void* page_alloc(const size_t bytes)
{
	const size_t blocks_needed = bytes / KB(4);

	void* virt_first;
	size_t blocks_found = 0;

	for (size_t dir_i = 0; dir_i < 1024; dir_i++)
	{
		page_entry* tbl = &page_directory[dir_i];
		if ((page_get_flags(tbl) & PAGE_PRESENT) != 0)
		{
			if (tbl != page_tbl)
			{
				for (size_t tbl_i = 0; tbl_i < 1024; tbl_i++)
				{
					page_entry* entry = &tbl[tbl_i];
					if ((page_get_flags(entry) & PAGE_PRESENT) == 0)
					{
						 if (blocks_found == 0)
						 {
							 blocks_found = 1;
							 virt_first = (void*) (tbl_i * KB(4)) + (dir_i * MB(4));
						 }
						 else
							 blocks_found++;
					}
					else
						blocks_found = 0;
				}
			}
		}
		else
		{
			// Page table does not exist yet, which means every block will be free.
			if (blocks_found == 0)
			{
				blocks_found = 1024;
				virt_first = (void*) (dir_i * MB(4));
			}
			else
			{
				blocks_found += 1024;
			}
			if (blocks_found >= blocks_needed)
				break;
		}
	}

	page_register_blocks(virt_first, blocks_needed);
	return NULL;
}

void* page_map(void* virt, void* phys, size_t length)
{
	page_entry* entry = page_get_entry(virt);
	page_set_address(entry, phys);
	page_set_flags(entry, PAGE_PRESENT | PAGE_RW);
	return virt;
}

void* page_idmap(void* addr, size_t length)
{
	return page_map(addr, addr, length);
}

void page_free(void* virt)
{
	kernel_panic("Not yet implemented");
}

static void page_init_set_tbl_entry(page_entry* tbl, void* addr)
{
	page_entry* entry = (page_entry*) tbl[(size_t)addr % MB(4) / KB(4)];
	page_set_address(entry, addr);
	page_set_flags(entry, PAGE_PRESENT | PAGE_RW);
}

static void page_init_set_dir_entry(page_entry* dir, void* addr)
{
	page_entry* entry = (page_entry*) dir[(size_t)addr / KB(4)];
	page_set_address(entry, addr);
	page_set_flags(entry, PAGE_PRESENT | PAGE_RW);
}

void page_init()
{
	// Create the directory and initialise it to all zeroes.
	// We need 12 KiB of initial storage; 4 KiB for the page
	// directory, 4 KiB for the page table containing the
	// entry that stores the page directory, and a final 4 KiB
	// For a page table that will contain mappings the every
	// page table. This map must be reserved for the pager itself.
	//page_directory = (page_entry*) kernel_find_next_free_mem(MINIMUM_SIZE);
	printf("Allocating\n");
	page_directory = (page_entry*) pmem_alloc(KB(4));
	page_tbl = (page_entry*) pmem_alloc(KB(4));
	page_entry* dir_tbl = (page_entry*) pmem_alloc(KB(4));

	if (page_directory == NULL)
		kernel_panic("Could not allocate page directory");
	if (dir_tbl == NULL)
		kernel_panic("Could not allocate initial page table");
	if (page_tbl == NULL)
		kernel_panic("Could not allocate meta page table");

	printf("Mem setting\n");
	memset(page_directory, 0, KB(4));
	printf("A");
	memset(page_tbl, 0, KB(4));
	printf("B");
	memset(dir_tbl, 0, KB(4));

	// Create the table that corresponds to the directory itself.
	// This way we can make sure that the directory and the
	// initial page table are id-mapped.
	printf("Setting tbl entries\n");
	page_init_set_tbl_entry(dir_tbl, page_directory);
	page_init_set_tbl_entry(page_tbl, dir_tbl);
	page_init_set_tbl_entry(page_tbl, page_tbl);

	// And link the table to the directory at the right location.
	// We can find this location by taking the directory address
	// and dividing this by 4 megabytes.
	printf("Setting dir entries\n");
	page_init_set_dir_entry(page_directory, dir_tbl);
	page_init_set_dir_entry(page_directory, page_tbl);
	page_tbl_offset = (void*) (page_tbl - ((size_t)page_tbl % MB(4)));

	printf("Done\n");
	/*page_entry* dir_entry = (page_entry*) page_directory[(size_t)page_directory / MB(4)];
	page_set_address(page_directory, dir_entry);
	page_set_flags(dir_entry, PAGE_PRESENT | PAGE_RW);*/

	// Before we can enable it the kernel will still need to add
	// it's own maps. Otherwise we might triple-fault the system.
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




























