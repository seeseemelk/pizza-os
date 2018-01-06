/*
 * kernel.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stddef.h>

size_t memory_available;
void kernel_panic(const char* fmt, ...);

/**
 * Finds the next free memory slot in the memory map given by the multiboot header.
 * Note that all the memory address that are returned by the function are physical
 * address. They should still be translated to take paging into account.
 */
//void* kernel_find_next_free_mem(size_t amount);

#endif /* KERNEL_H_ */
