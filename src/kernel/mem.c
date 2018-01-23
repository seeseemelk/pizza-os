/*
 * mem.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "mem.h"
#include "cdefs.h"
#include "kernel.h"
#include "page.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BLK_SIZE KB(4)
#define TBL_SIZE MB(4)
#define TBLI(x) (((size_t)x) % TBL_SIZE / BLK_SIZE)
#define DIRI(x) (((size_t)x) / MB(4))

#define FREE 0
#define USED -1
typedef unsigned char state_t;

typedef struct dir_t dir_t;
typedef struct tbl_t tbl_t;

struct dir_t
{
	tbl_t* tbl;
};

struct tbl_t
{
	size_t state;
};

dir_t* directory;

/**
 * Get a table from the directory.
 * This is actually a pointer to the first entry.
 */
tbl_t* mem_get_tbl(void* mem)
{
	size_t dir_i = DIRI(mem);

	if (directory[dir_i].tbl == FREE)
	{
		tbl_t* tbl = (tbl_t*) page_alloc(sizeof(tbl_t) * 1024);
		memset(tbl, 0, sizeof(tbl_t) * 1024);
		directory[dir_i].tbl = tbl;
		return tbl;
	}
	else
	{
		tbl_t* tbl = directory[dir_i].tbl;
		return tbl;
	}
}

/**
 * Gets a pointer to a table entry.
 */
tbl_t* mem_get_tbl_entry(void* mem)
{
	size_t tbl_i = TBLI(mem);

	tbl_t* tbl = mem_get_tbl(mem);
	return tbl + tbl_i;
}

void* mem_alloc(const size_t bytes)
{
	// We allocate the pages.
	void* mem = page_alloc(bytes);

	// We set first table entry so that it contains the size of the allocated memory.
	tbl_t* first_entry = mem_get_tbl_entry(mem);
	first_entry->state = bytes;

	// Then we set the state of the following entries to USED
	// This only has to be done if we are allocating more than one block.
	if (bytes / BLK_SIZE > 0)
	{
		size_t blocks = ceildiv(bytes, BLK_SIZE);
		for (size_t block = 1; block < blocks; block++)
		{
			tbl_t* tbl_entry = mem_get_tbl_entry((void*)((size_t)mem + block * BLK_SIZE));
			tbl_entry->state = USED;
		}
	}

	return mem;
}

void mem_free(void* addr)
{
	tbl_t* first_entry = mem_get_tbl_entry(addr);
	size_t size = first_entry->state;
	size_t blocks = size / BLK_SIZE;
	first_entry->state = FREE;

	for (size_t i = 1; i < blocks; i++)
	{
		tbl_t* entry = mem_get_tbl_entry((void*)((size_t)addr + i * BLK_SIZE));
		entry->state = FREE;
	}
}

void mem_init()
{
	directory = page_alloc(sizeof(dir_t) * 1024);
	memset(directory, 0, sizeof(dir_t) * 1024);
}

/*void* mem_base;
size_t mem_length;

state_t* mem_map;
size_t map_entries;

void* mem_alloc(const size_t size)
{
	const size_t required_size = size + sizeof(size_t);
}

void mem_free(void* address)
{
	kernel_panic("Not implemented");
}

void* mem_realloc(void* address, size_t new_size)
{
	kernel_panic("Not implemented");
	return NULL;
}

void mem_init()
{
	// We split the map into enough entries so that the entire page table fits inside it.

	// First we set the initial number of entries we want in the table.
	mem_length = 1024;
	map_entries = (GB(4) / BLOCK_SIZE);
	mem_map = page_alloc(map_entries * sizeof(state_t));
	memset(mem_map, 0, map_entries * sizeof(state_t));
}
*/



















