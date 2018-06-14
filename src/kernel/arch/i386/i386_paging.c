/*
 * page.c
 *
 *  Created on: Jan 7, 2018
 *      Author: seeseemelk
 */
#include "i386_asm.h"
#include "pmem.h"
#include "kernel.h"
#include "cdefs.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "i386_paging.h"

#define KERNEL_START GB(3)
#define PAGE_TABLES_START GB(4) - MB(4)
#define ENTRY_ADDRESS(x) (virt_addr_t)((size_t)x >> 12)
#define REAL_ADDRESS(x) (virt_addr_t)((size_t)x << 12)

typedef struct
{
	u32 present : 1;
	u32 writable : 1;
	u32 userspace : 1;
	u32 pwt : 1; // Page-level write-through: ???
	u32 pcd : 1; // Page-level cache disable: ???
	u32 accessed : 1;
	u32 IGNORE1 : 1;
	u32 large_page : 1; // 1 if it maps a 4-MiB page, 0 if it references a page table.
	u32 IGNORE2 : 1;
	u32 address : 20;
} page_dir_entry;

typedef struct
{
	u32 present : 1;
	u32 writable : 1;
	u32 userspace : 1;
	u32 pwt : 1; /* Page-level write-through: ??? */
	u32 pcd : 1; /* Page-level cache disable: ??? */
	u32 accessed : 1;
	u32 dirty : 1;
	u32 pat : 1; /* ???, but only supported since PIII */
	u32 global : 1; /* ??? */
	u32 IGNORE1 : 3;
	u32 address : 20;
} page_table_entry;

/* The main page directory */
page_dir_entry* page_directory;
phys_addr_t phys_current;
/* The page table that contains the other page tables. */
page_table_entry page_metatable[1024];

phys_addr_t page_get_virt_addr(virt_addr_t addr)
{
	page_dir_entry* dentry = page_directory + ((size_t) addr / MB(4));
	if (dentry->present)
	{
		if (dentry->large_page)
		{
			size_t offset = (size_t) addr % MB(4);
			return (phys_addr_t) (REAL_ADDRESS(dentry->address) + offset);
		}
		else
		{
			size_t index = (size_t) addr % MB(4) / KB(4);
			size_t offset = (size_t) addr % KB(4);
			page_table_entry* tentry = (page_table_entry*) (PAGE_TABLES_START + index);
			return (phys_addr_t) (REAL_ADDRESS(tentry->address) + offset);
		}
	}
	else
		kernel_panic("Get physical address of non-present page directory entry");
	return NULL;
}

void page_reload_all()
{
	asm_load_cr3_page_dir(phys_current);
}

extern unsigned int _system_page_directory;
paged_t* arch_page_init()
{
	/* First we find the current page directory. */
	page_directory = (page_dir_entry*) &_system_page_directory;
	phys_current = ((phys_addr_t) page_directory) - 0xC0000000;

	/* We set all the bytes to 0 to make sure all entries are marked as not-present. */
	memset(page_metatable, 0, KB(4));

	/* Add the metatable to itself as the very last entry. */
	page_metatable[1023].address = (u32) page_metatable;
	page_metatable[1023].writable = 1;
	page_metatable[1023].present = 1;

	/* Add the metatable to the page directory. */
	page_directory[1023].address = (u32) page_metatable;
	page_directory[1023].writable = 1;
	page_directory[1023].present = 1;

	/* Disable the old low-memory page table entry. */
	page_directory[0].present = 0;

	page_dir_entry* entry = page_directory + (GB(3) / MB(4));
	entry->large_page = 1;
	entry->address = 0;
	entry->present = 1;
	entry->writable = 1;

	/* Enable paging */
	page_reload_all();

	phys_current = page_get_virt_addr((virt_addr_t) page_directory);

	return page_directory;
}

void arch_page_enable()
{
	// Before we turn on paging we should allocate a page
	// for the memory window. We are going to allocate it
	// right before the metatable.
	/*page_alloc_page((void*) (1023 * MB(4) - KB(4)), (void*) mem_window);

	// Set this variable to true
	paging_enabled = true;

	// Also allocate some physical memory for the memory window.
	//page_idmap(mem_window, KB(4));

	// Load the address of the page directory into CR3
	asm_load_cr3_page_dir((size_t) page_directory);

	// Set CR0.PG to 1.
	// This will put the CPU in 32-bit paging mode.
	// (see Intel Manual Volume 3, 4.1.2)
	asm_enable_cr0_pepg();*/

	// Get the virtual address to the metatable.
	/*page_metatable = (page_entry*) (1023 * MB(4));

	// Now we still need to get the handle for the memory window.
	page_entry* tbl = page_get_table((void*) (1022 * MB(4)));
	mem_window_entry = (page_entry*)(tbl + 1023);
	mem_window = (void*) (1023 * MB(4) - KB(4));*/

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















