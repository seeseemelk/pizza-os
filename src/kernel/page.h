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

/**
 * Allocates free pages so that a total of bytes length worth of memory
 * can be addressed. It will return the virtual address to the memory.
 */
void* page_alloc(size_t bytes);

/**
 * Allocates free pages so that a total of bytes length worth of memory
 * can be addressed and so that the virtual address accesses the specified
 * physical addresses. It will return the virtual address to the memory.
 */
void* page_alloc_phys(void* phys, size_t bytes);

/**
 * Maps a specific virtual address to a specific physical address for
 * a length of bytes. It will return the virtual address to the memory.
 */
void* page_map(void* virt, void* phys, size_t bytes);

/**
 * ID maps bytes worth of pages and return the virtual address to the memory.
 */
void* page_idmap(void* addr, size_t bytes);

/**
 * Frees bytes worth of pages at the given virtual address.
 */
void page_free(void* virt, size_t bytes);

/**
 * Get the length of bytes that a specific page can address.
 */
size_t page_length(void* virt);

/**
 * Get the physical address a virtual address maps to.
 */
void* page_get_phys_address(void* virt);

/**
 * Sets the physical address a virtual address maps to.
 * Note that it will map the entire page.
 */
void page_set_phys_address(void* virt, void* phys);

typedef unsigned char* phys_addr_t;
typedef unsigned char* virt_addr_t;

typedef enum action_t action_t;
typedef struct query_t query_t;
typedef struct page_t page_t;

enum action_t
{
	PAGE_RESERVE = 0x01,
	PAGE_ALLOC =   0x02,
	PAGE_FREE =    0x04,
	PAGE_LOWMEM =  0x08,
	PAGE_USER =    0x0F,
	PAGE_KERNEL =  0x10
};

struct query_t
{
	virt_addr_t begin;
	size_t bytes;
	action_t action;
};

struct page_t
{
	virt_addr_t begin;
	unsigned int pages;
	unsigned int bytes_per_page;
};

phys_addr_t page_phys_addr(virt_addr_t begin);
bool page_query(page_t* page, query_t* query);
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































