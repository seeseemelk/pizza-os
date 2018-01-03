/*
 * page.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef PAGE_H_
#define PAGE_H_

#if TARGET==PC
#include "arch/pc/page.h"
#endif

#include <stddef.h>

/**
 * Allocates a single page.
 */
page* page_alloc();

/**
 * Allocates a single page so that the page contains
 * the specified physical address.
 */
page* page_alloc_phys(void* phys_addr);

/**
 * Allocates a single page so that the page contains
 * the specified virtual address.
 */
page* page_alloc_virt(void* virt_addr);

/**
 * Allocates a single page so that the specified virtual address
 * accesses the specified physical address.
 */
page* page_alloc_over(void* phys_addr, void* virt_addr);

/**
 * Frees a certain page.
 */
void page_free(page* p);

#endif /* PAGE_H_ */
