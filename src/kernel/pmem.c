/*
 * pmem.c
 *
 *  Created on: Jan 5, 2018
 *      Author: seeseemelk
 */
#include "pmem.h"
#include "page.h"
#include "config.h"
#include "kernel.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

state_t* pmem_map;
size_t pmem_size;

void* pmem_alloc(size_t amount)
{
	const size_t blocks_needed = ((amount - 1) / PMEM_BLOCK_SIZE) + 1;

	size_t index_found;
	size_t blocks_found = 0;

	for (size_t i = 0; i < pmem_size; i++)
	{
		if (pmem_map[i] == PMEM_FREE)
		{
			if (blocks_found == 0)
			{
				blocks_found = 1;
				index_found = i;
			}
			else
				blocks_found++;

			if (blocks_found == blocks_needed)
				break;
		}
	}

	if (blocks_found >= blocks_needed)
	{
		for (size_t i = index_found; i < index_found + blocks_needed; i++)
			pmem_map[i] = PMEM_USED;
		return (void*) (index_found * PMEM_BLOCK_SIZE);
	}
	else
	{
		printf("WARNING! No more blocks left\n");
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
	const size_t start_index = (size_t)start / PMEM_BLOCK_SIZE * sizeof(state_t);
	const size_t blocks = amount / PMEM_BLOCK_SIZE;
	for (size_t i = start_index; i < start_index + blocks; i++)
			pmem_map[i] = state;
}

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
	printf("Map starts at block %d and is %d long\n", map_start, blocks);
	for (size_t block = 0; block < blocks; block++)
		pmem_map[block + map_start] = PMEM_USED;
}

void pmem_register_pages()
{
	page_idmap(pmem_map, ceilg(pmem_size * sizeof(state_t), KB(4)));
}


































