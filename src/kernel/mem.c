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
//#define USED 1

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

/**
 * Gets for a given address the index into sizes for that location of memory.
 */
size_t mem_get_sizeindex_for(void* addr)
{
	for (size_t i = 0; i < COUNT; i++)
	{
		void* mem_start = bitmap_mem[i];
		void* mem_end = bitmap_mem[i] + sizes[i]*1024;
		if (addr >= mem_start && addr < mem_end)
			return i;
	}
	kernel_panic("Could not find sizeindex for address 0x%X", addr);
	return 0;
}

/**
 * Gets the bitmap that contains the mapping for a specific address.
 */
bitmap_t* mem_get_bitmap_for(void* addr)
{
	size_t size_index = mem_get_sizeindex_for(addr);
	u8* caddr = addr;
	u8* begin_addr = bitmap_mem[size_index];
	bitmap_t* bitmap = bitmaps[size_index];
	size_t bitmap_index = (caddr - begin_addr) / sizes[size_index];
	return bitmap + bitmap_index;
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
		bitmap[i] = bitmap[i-1] - 1;

	return mem_get_memory_in(index, first_block, blocks_needed);
}

void mem_free(void* address)
{
	bitmap_t* bitmap = mem_get_bitmap_for(address);
	if (*bitmap == FREE)
		kernel_panic("Double free");

	const size_t count = *bitmap;
	for (size_t i = 0; i < count; i++)
		*bitmap = 0;
}

void* mem_realloc(void* address, size_t new_size)
{
	size_t size_index = mem_get_sizeindex_for(address);
	bitmap_t* bitmap = mem_get_bitmap_for(address);
	size_t old_size = (*bitmap) * sizes[size_index];

	size_t old_blocks = old_size / sizes[size_index];
	size_t new_blocks = new_size / sizes[size_index];

	if (new_size < old_size)
	{
		// The size of the allocated memory has been reduced.
		size_t delta = old_blocks - new_blocks;
		for (size_t i = 0; i < new_blocks; i++)
			bitmap[i] -= delta;
		for (size_t i = new_blocks; i < old_blocks; i++)
			bitmap[i] = FREE;
		// We keep the old address
		return address;
	}
	else
	{
		bool can_increase = true;
		// Check if the size can simple be increased.
		for (size_t i = old_blocks; i < new_blocks; i++)
		{
			if (bitmap[i] != FREE)
			{
				can_increase = false;
				break;
			}
		}

		if (can_increase)
		{
			// We simply add a few blocks.
			for (size_t i = 0; i < new_blocks; i++)
				bitmap[i] = new_blocks - i;
			return mem_get_memory_in(size_index, bitmap - bitmaps[size_index], new_blocks);
		}
		else
		{
			// We reallocate.
			void* new_address = mem_alloc(new_size);
			memcpy(new_address, address, old_size);
			mem_free(address);
			return new_address;
		}
	}

	return NULL;
}

/**
 * Allocates a single 4-KB page.
 */
void* mem_create_page(int pages, action_t options)
{
	page_t result;
	page_query(&result, 0, KB(4) * pages, options);
	return result.begin;
}

/**
 * Initialises a bitmap.
 */
void mem_init_bitmap(int index)
{
	const size_t size = sizes[index];
	const size_t pages = size / 4;
	bitmap_mem[index] = mem_create_page(pages, PAGE_GLOBAL);
	bitmaps[index] = mem_create_page(1, PAGE_GLOBAL | PAGE_ALLOCATE);
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



















