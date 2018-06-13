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
#define ENTRY_ADDRESS(x) (virt_addr_t*)((size_t)x >> 12)

typedef struct
{
	u32 present : 1;
	u32 writable : 1;
	u32 userspace : 1;
	u32 pwt : 1; /* Page-level write-through: ??? */
	u32 pcd : 1; /* Page-level cache disable: ??? */
	u32 accesed : 1;
	u32 IGNORE1 : 1;
	u32 large_page : 1; /* 1 if it maps a 4-MiB page, 0 if it references a page table. */
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
/* The page table that contains the other page tables. */
page_table_entry* page_metatable;
bool paging_enabled = false;

/**
 * Gets the page table entry that describes a specific virtual address.
 */
page_table_entry* page_get_page_entry(virt_addr_t addr)
{
	if (!paging_enabled)
	{
		page_table_entry* ptable = (page_table_entry*) (page_directory[(size_t)addr / MB(4)].address >> 12);
		return ptable + ((size_t)addr % MB(4));
	}
	else
	{
		/* To be created */
		return NULL;
	}
}

void page_single_idmap(virt_addr_t addr)
{
	page_table_entry* entry = page_get_page_entry(addr);
	entry->address = (size_t) ENTRY_ADDRESS(addr);
	entry->writable = 1;
}

void page_idmap(virt_addr_t start, size_t pages)
{
	for (size_t i = 0; i < pages; i++)
		page_single_idmap(start + i * KB(4));
}

paged_t* arch_page_init()
{
	/* First we allocate the page directory and the metatable. */
	page_directory = pmem_alloc(KB(4));
	if (page_directory == NULL)
		kernel_panic("Could not allocate page directory");
	page_metatable = pmem_alloc(KB(4));
	if (page_metatable == NULL)
		kernel_panic("Could not allocate metatable");

	/* We set all the bytes to 0 to make sure all entries are marked as not-present. */
	memset(page_directory, 0, KB(4));
	memset(page_metatable, 0, KB(4));

	/* Add the metatable to itself as the very last entry. */
	page_metatable[1023].address = (u32) page_metatable;
	page_metatable[1023].writable = 1;
	page_metatable[1023].present = 1;

	/* Add the metatable to the page directory. */
	page_directory[1023].address = (u32) page_metatable;
	page_directory[1023].writable = 1;
	page_directory[1023].present = 1;

	/* Add an entry in the page directory for a higher half kernel */
	page_directory[GB(3) / MB(4)].large_page = 1;
	page_directory[GB(3) / MB(4)].address = 0;
	page_directory[GB(3) / MB(4)].present = 1;

	// Finally we ID-map the page directory.
	page_idmap((virt_addr_t) page_directory, KB(4));

	return (paged_t*) page_directory;
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















