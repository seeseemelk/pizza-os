/*
 * pmem.h
 * This header has several functions to interact with the physical memory allocator.
 * The physical memory allocator keeps track of physical memory.
 *
 *  Created on: Jan 5, 2018
 *      Author: seeseemelk
 */
#include <stddef.h>

#define PMEM_FREE 0
#define PMEM_USED 1
#define PMEM_RESERVED 2

typedef char state_t;

void* pmem_alloc(size_t amount);
void pmem_set(void* start, size_t amount, state_t state);
void pmem_init(void* start, size_t mem_size);
void pmem_register_pages();

