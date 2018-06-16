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

#define COUNT (sizeof(sizes)/sizeof(size_t))

#define FREE 0
#define USED 1

typedef char bitmap_t;

const size_t sizes[] = {4, 16, 32, 64, 128, 512};
size_t allocated[COUNT];
bitmap_t* bitmaps[COUNT];
void* bitmap_mem[COUNT];

/**
 * Gets the index for a given size.
 */
size_t mem_get_index_for(size_t size)
{
	for (size_t i = 0; i < COUNT; i++)
	{
		if (size <= sizes[i])
			return i;
	}
	return COUNT - 1;
}

/**
 * Gets the real memory used for a specific block.
 */
void* mem_get_memory_in(size_t index, size_t block_start, size_t blocks)
{
	void* addr = bitmap_mem[index] + block_start*sizes[index];
	size_t needs_allocated = (block_start+blocks)*sizes[index];
	while (allocated[index] < needs_allocated)
	{
		page_allocate((u8*)bitmap_mem[index] + allocated[index], 1);
		allocated[index] += KB(4);
	}
	return addr;
}

void* mem_alloc(size_t size)
{
	if (size == 0)
		kernel_panic("Allocating 0 bytes");

	size_t index = mem_get_index_for(size);

	bitmap_t* bitmap = bitmaps[index];

	size_t blocks_needed = ceildiv(size, sizes[index]);
	size_t blocks_found = 0;
	size_t first_block;

	//for (size_t i = 0; i < 4096; i++)
	size_t i = 0;
	while (i < 4096)
	{
		if (bitmap[i] == FREE)
		{
			if (blocks_found == 0)
				first_block = i;
			blocks_found++;
			i++;

			if (blocks_found == blocks_needed)
				break;
		}
		else
		{
			blocks_found = 0;
			i += bitmap[i];
		}
	}

	if (blocks_found < blocks_needed)
		kernel_panic("Not enough memory");

	bitmap[first_block] = blocks_needed;
	for (size_t i = first_block+1; i < blocks_needed + first_block; i++)
	{
		bitmap[i] = USED;
	}

	return mem_get_memory_in(index, first_block, blocks_needed);
}

void mem_free(void* address)
{
	kernel_log("Freeing");
	UNUSED(address);
}

void* mem_realloc(void* address, size_t new_size)
{
	kernel_panic("Reallocing");
	UNUSED(address);
	UNUSED(new_size);
	return NULL;
}

/**
 * Allocates a single 4-KB page.
 */
void* mem_create_page(int pages)
{
	page_t result;
	page_query(&result, 0, KB(4) * pages, PAGE_GLOBAL | PAGE_ALLOCATE);
	return result.begin;
}

/**
 * Initialises a bitmap.
 */
void mem_init_bitmap(int index)
{
	const size_t size = sizes[index];
	const size_t pages = size / 4;
	bitmap_mem[index] = mem_create_page(pages);
	bitmaps[index] = mem_create_page(1);
	allocated[index] = 0;
	memset(bitmaps[index], FREE, KB(4));
}

void mem_init()
{
	for (size_t i = 0; i < COUNT; i++)
	{
		mem_init_bitmap(i);
	}
}



















