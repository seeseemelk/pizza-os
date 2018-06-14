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
#define KERNEL_MEM_END GB(3) + MB(4)
#define PAGE_TABLES_START GB(4) - MB(4)
#define ENTRY_ADDRESS(x) (virt_addr_t)((size_t)(x) >> 12)
#define REAL_ADDRESS(x) (virt_addr_t)((size_t)(x) << 12)

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
page_table_entry main_page_metatable[1024] __attribute__ ((aligned (4096)));
page_table_entry* page_metatable = (page_table_entry*) PAGE_TABLES_START;

phys_addr_t page_phys_addr(virt_addr_t addr)
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
	memset(main_page_metatable, 0, KB(4));

	/* Add the metatable to itself as the very last entry. */
	//main_page_metatable[1023].address = (u32) ENTRY_ADDRESS(page_phys_addr((virt_addr_t) main_page_metatable));
	//main_page_metatable[50].address = (u32) ENTRY_ADDRESS((virt_addr_t)main_page_metatable - 0xC0000000);
	main_page_metatable[50].address = 0x0;
	main_page_metatable[50].writable = 1;
	main_page_metatable[50].present = 1;
	//main_page_metatable[1020] = main_page_metatable[1023];

	/* Add the metatable to the page directory. */
	//page_directory[1020].address = (u32) ENTRY_ADDRESS(page_phys_addr((virt_addr_t) main_page_metatable));
	//page_directory[1020].address = (u32) ENTRY_ADDRESS((virt_addr_t)main_page_metatable - 0xC0000000);
	page_directory[4].address = (u32) ENTRY_ADDRESS((virt_addr_t)main_page_metatable - 0xC0000000);
	page_directory[4].writable = 1;
	page_directory[4].present = 1;

	/* Disable the old low-memory page table entry. */
	//page_directory[0].present = 0;

	/* Enable paging */
	page_reload_all();

	while(1);

	return page_directory;
}

typedef struct
{
	page_table_entry* entry;
	// These two point to the next entry to read.
	unsigned short dir_index;
	unsigned short table_index;
	unsigned short dir_index_end;
	unsigned short table_index_end;

	// Does the page_table_entry exist at all
	bool exists;
} page_it;

page_it page_iterate(virt_addr_t begin, virt_addr_t end)
{
	page_it it;
	it.dir_index = (unsigned short) ((size_t) begin / MB(4));
	it.table_index = (unsigned short) ((size_t) begin % MB(4) / KB(4));
	it.dir_index_end = (unsigned short) ((size_t) end / MB(4));
	it.table_index_end = (unsigned short) ((size_t) end % MB(4) / KB(4));
	return it;
}

bool page_it_next(page_it* it)
{
	if (it->dir_index >= it->dir_index_end && it->table_index >= it->table_index_end)
		return false;

	it->entry = ((page_table_entry*) PAGE_TABLES_START) + (it->dir_index * 1024) + it->table_index;
	it->exists = page_directory[it->dir_index].present;

	if (it->table_index == 1023)
	{
		it->dir_index++;
		it->table_index = 0;
	}
	else
		it->table_index++;

	return true;
}

virt_addr_t page_it_virt_addr(page_it* it)
{
	return (virt_addr_t) ((it->dir_index * MB(4)) + (it->table_index * KB(4)));
}

void page_it_create(page_it* it)
{
	phys_addr_t addr = (phys_addr_t) pmem_alloc(4096);
	page_table_entry* entry = page_metatable + it->dir_index;
	entry->address = (u32) ENTRY_ADDRESS(addr);
	entry->writable = 1;
	entry->present = 1;
	it->exists = true;
}

bool page_query(page_t* page, size_t align, size_t bytes, action_t action)
{
	/*
	 * Check if we have to find a page in user space memory or kernel space memory
	 */
	if ((action & PAGE_GLOBAL) == 0)
	{
		kernel_panic("Not yet supported");
		return false;
	}
	else
	{
		/* Find free pages */

		size_t blocks_needed = ceildiv(bytes, KB(4));
		size_t blocks_found = 0;
		page_it blocks_first;
		page_it it = page_iterate((virt_addr_t) KERNEL_MEM_END, (virt_addr_t) PAGE_TABLES_START);

		while (page_it_next(&it))
		{
			if (it.exists == false || it.entry->present == 0)
			{
				// First new block found
				if (blocks_found == 0)
				{
					// Has to be aligned
					if ((size_t)page_it_virt_addr(&it) % align != 0)
						continue;
					blocks_first = it;
				}

				if (++blocks_found >= blocks_needed)
					break;
			}
		}

		if (blocks_found >= blocks_needed)
		{
			it = blocks_first;
			page->begin = page_it_virt_addr(&it);
			page->bytes_per_page = 4096;
			page->pages = blocks_needed;
			for (size_t i = 0; i < blocks_needed; i++)
			{
				if (it.exists == false)
					page_it_create(&it);

				it.entry->present = 1;

				if (action & PAGE_READONLY)
					it.entry->writable = 0;
				else
					it.entry->writable = 1;

				if (action & PAGE_USER)
					it.entry->userspace = 1;
				else
					it.entry->userspace = 0;

				page_it_next(&it);
			}
			return true;
		}
		else
		{
			kernel_panic("Not enough kernel memory available");
			return false;
		}
	}
}

void arch_page_free(page_t* page);
void arch_page_assign(virt_addr_t page, phys_addr_t phys_addr);
paged_t* arch_page_copy(paged_t* page_directory);
void arch_page_load(paged_t* page_directory);















