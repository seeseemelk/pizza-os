/*
 * mem.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

/**
 * Allocates a certain amount of memory anywhere.
 */
//void* mem_alloc(size_t size);

/**
 * Frees a certain amount of memory somewhere.
 */
//void mem_free(void* address);

/**
 * Reallocates a block of memory so that it has a new size.
 */
//void* mem_realloc(void* address, size_t new_size);

/**
 * Initiates the memory allocator.
 * The caller should give a range of memory address that can be used by the memory allocator.
 */
//void mem_init(size_t address, size_t size);

/**
 * Should be called after paging has been enabled.
 */
//void mem_paging_enabled();

/**
 * Initialises the memory allocator.
 * The total size of the memory should be passed
 */
//size_t mem_init(size_t memory_size);

#endif /* MEM_H_ */












