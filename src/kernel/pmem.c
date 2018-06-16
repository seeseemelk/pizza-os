/*
 * pmem.c
 *
 *  Created on: Jan 5, 2018
 *      Author: seeseemelk
 */
#include "pmem.h"
#include "config.h"
#include "kernel.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "paging.h"

state_t* pmem_map;
size_t pmem_size;

void* pmem_alloc(size_t amount)
{
	const size_t blocks_needed = ((amount - 1) / PMEM_BLOCK_SIZE) + 1;

	size_t index_found = 0;
	size_t blocks_found = 0;

	for (size_t i = 0; i < pmem_size; i++)
	{
		if (pmem_map[i] == PMEM_FREE)
		{
			if (blocks_found == 0)
				index_found = i;
			blocks_found++;

			if (blocks_found == blocks_needed)
				break;
		}
		else
			blocks_found = 0;
	}

	if (blocks_found >= blocks_needed)
	{
		for (size_t i = index_found; i < index_found + blocks_needed; i++)
			pmem_map[i] = PMEM_USED;
		return (void*) (index_found * PMEM_BLOCK_SIZE);
	}
	else
	{
		kernel_panic("WARNING! No more blocks left\n");
		return NULL;
	}
}

void pmem_free(void* start, size_t amount)
{
	const size_t index = (size_t)start / PMEM_BLOCK_SIZE;
	const size_t blocks = amount / PMEM_BLOCK_SIZE;
	for (size_t i = index; i < index + blocks; i++)
		pmem_map[i] = PMEM_FREE;
}

void pmem_set(void* start, size_t amount, state_t state)
{
	const size_t start_index = (size_t)start / PMEM_BLOCK_SIZE;
	const size_t blocks = ceildiv(amount, PMEM_BLOCK_SIZE);
	for (size_t i = start_index; i < start_index + blocks; i++)
	{
		if (i < pmem_size)
			pmem_map[i] = state;
	}
}

/**
 * Initialises the memory allocator.
 * A pointer should be passed to where it is allowed to place the memory map,
 * and the total available memory size should be passed on.
 */
void pmem_init(void* start, size_t mem_size)
{
	// Create the map and set it to FREE.
	pmem_map = (state_t*) start;
	pmem_size = mem_size / PMEM_BLOCK_SIZE;
	for (size_t i = 0; i < pmem_size; i++)
		pmem_map[i] = PMEM_FREE;

	// Then set the blocks of the map itself to USED
	size_t map_start = (size_t)pmem_map / PMEM_BLOCK_SIZE;
	size_t blocks = ceildiv(pmem_size, PMEM_BLOCK_SIZE);
	for (size_t block = 0; block < blocks; block++)
		pmem_map[block + map_start] = PMEM_USED;
}

/*
void pmem_register_pages()
{
	page_idmap(pmem_map, ceilg(pmem_size * sizeof(state_t), KB(4)));
}
*/


































