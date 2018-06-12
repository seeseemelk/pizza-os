/*
 * page.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef PAGE_H_
#define PAGE_H_

#if TARGET==i386
#include <arch/i386/i386_page.h>
#endif

#include <stddef.h>
#include <stdbool.h>

/** Represents a pointer to somewhere in physical memory. */
typedef unsigned char* phys_addr_t;
/** Represents a pointer to somewhere in virtual memory. */
typedef unsigned char* virt_addr_t;

/** The action the pager should take. */
typedef enum action_t action_t;
/** Represents a set of pages. */
typedef struct page_t page_t;

enum action_t
{
	/** Assign physical memory to the page. */
	PAGE_ALLOC =    0x02,
	/** Allows userspace to access the memory. */
	PAGE_USER =     0x04,
	PAGE_READONLY = 0x08
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
 */
phys_addr_t page_phys_addr(virt_addr_t begin);

/**
 * Queries
 */
bool page_query(page_t* page, virt_addr_t begin, size_t bytes, action_t action);
void page_free(page_t* page);
void page_assign(virt_addr_t page, phys_addr_t phys_addr);

/*
void exampleCode()
{
	// Find memory and reserve it
	query_t query;
	page_t page;
	query.bytes = 9000;
	query.action = PAGE_ALLOC;
	if (!page_query(&page, &query))
		kernel_panic("Out of memory");

	// Reserve extra after already commited
	query.action = PAGE_RESERVE;
	query.bytes = 400;
	query.begin = page.begin + page.pages * page.bytes_per_page;
	if (page_query(&page, &query))
		kernel_log("We have to exted some other way");
}
*/

#endif /* PAGE_H_ */































