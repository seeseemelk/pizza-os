/**
 * A buddy-memory allocator.
 */
#include "mem.h"
#include "pmem.h"
#include "cdefs.h"
#include "config.h"
#include "kernel.h"
#include "paging.h"
#include <math.h>
#include <string.h>

#define MEM_BLOCK_SIZE 16
#define METADATA_SIZE KB(4)

#define FREE 0
#define USED 1

typedef struct
{
	/** State of the entry. */
	char state;
} buddy_t;

/*
 * Example 1:
 *                 ORDER 0
 *                   /\
 *                  /  \
 *                 /    \
 *         ORDER 1.1    ORDER 1.2
 *         /     \       /     \
 *       O2.1   O2.2   O2.3   O2.4
 *
 * list = [0, 1.1, 2.1, 2.2, 1.2, 2.3, 2.4]; (length = 7)
 */

buddy_t* list;

u8* kernel_memory;

buddy_t* find_buddy_of_order(buddy_t* buddy, size_t list_size, size_t order, size_t wanted_order,
		size_t mem_begin, size_t mem_length, size_t* mem_out, size_t* mem_out_length)
{
	if (order == wanted_order)
	{
		if (buddy->state == FREE)
		{
			*mem_out = mem_begin;
			*mem_out_length = mem_length;
			return buddy;
		}
		else
			return NULL;
	}
	else if (buddy->state == USED)
		return NULL;

	buddy_t* buddyA = buddy + 1;

	buddy_t* result = find_buddy_of_order(buddyA, list_size/2, order+1, wanted_order, mem_begin, mem_length / 2, mem_out, mem_out_length);
	if (result != NULL)
		return result;

	buddy_t* buddyB = (buddy_t*) (list_size / 2) + 1;
	result = find_buddy_of_order(buddyB, list_size/2, order+1, wanted_order, mem_begin + mem_length / 2, mem_length / 2, mem_out, mem_out_length);

	if (result != NULL)
		return result;
	else
		return NULL;
}

void* mem_alloc(size_t size)
{
	size_t wanted_order = log2(size);
	size_t mem_begin = (size_t) kernel_memory;
	size_t mem_length = KERNEL_MEMORY_SIZE;
	size_t mem_out;
	size_t mem_out_length;
	buddy_t* buddy = find_buddy_of_order(list+0, METADATA_SIZE, 0, wanted_order, mem_begin, mem_length, &mem_out, &mem_out_length);
	if (buddy == 0)
		kernel_panic("Out of buddies");

	// Map from buddy to real memory.
	size_t test = buddy - list;
	UNUSED(test);
	buddy->state = USED;
	return NULL;
}

void mem_free(void* address)
{
	UNUSED(address);
}

void* mem_realloc(void* address, size_t new_size)
{
	UNUSED(address);
	UNUSED(new_size);
	return NULL;
}

void mem_init()
{
	// Initialise the metadata

	// Gets some pages for the metadata.
	page_t result;
	page_query(&result, 0, METADATA_SIZE, PAGE_GLOBAL);
	list = (buddy_t*) result.begin;

	// Allocate physical memory for the metadata.
	void* mem = pmem_alloc(METADATA_SIZE);
	page_assign(list, mem);

	memset(list, 0, METADATA_SIZE);

	// Initialise the page store
	page_query(&result, 0, KERNEL_MEMORY_SIZE, PAGE_GLOBAL);
	kernel_memory = (u8*) result.begin;
}



















