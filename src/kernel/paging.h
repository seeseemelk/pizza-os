/*
 * page.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef PAGING_H_
#define PAGING_H_

#if TARGET==i386
#include "arch/i386/i386_paging.h"
#endif

#include "cdefs.h"
#include <stddef.h>
#include <stdbool.h>

/** A page directory. */
typedef void paged_t;

/** Represents a pointer to somewhere in physical memory. */
typedef u8* phys_addr_t;
/** Represents a pointer to somewhere in virtual memory. */
typedef u8* virt_addr_t;

/** The action the pager should take. */
typedef enum action_t action_t;
/** Represents a set of pages. */
typedef struct page_t page_t;

extern paged_t* kernel_page;

enum action_t
{
	/** Assigns the page in the globally shared memory space. */
	PAGE_GLOBAL =   0x01,
	/** Allows userspace to access the memory. */
	PAGE_USER =     0x02,
	/** Prevents writes to the page from happening. */
	PAGE_READONLY = 0x04
};

struct page_t
{
	/** The virtual address of the first page. */
	virt_addr_t begin;
	/** The number of pages. */
	unsigned short pages;
	/** The number of bytes per page. */
	unsigned short bytes_per_page;
};

/**
 * Requests the physical address of a virtual address.
 * This is may be needed for the memory allocator amongst other things.
 */
phys_addr_t page_phys_addr(virt_addr_t begin);

bool page_query(page_t* page, size_t align, size_t bytes, action_t action);
void page_free(page_t* page);
void page_assign(virt_addr_t page, phys_addr_t phys_addr);
void page_init();
paged_t* page_copy(paged_t* page_directory);
void page_load(paged_t* page_directory);

/*
 * To be implemented by the architecture.
 */

/**
 * Creates the basic kernel page directory.
 */
paged_t* arch_page_init();
/*
bool arch_page_query(page_t* page, size_t align, size_t bytes, action_t action);
void arch_page_free(page_t* page);
void arch_page_assign(virt_addr_t page, phys_addr_t phys_addr);
paged_t* arch_page_copy(paged_t* page_directory);
void arch_page_load(paged_t* page_directory);
*/
#endif /* PAGING_H_ */































