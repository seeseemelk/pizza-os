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

#endif /* PAGE_H_ */
