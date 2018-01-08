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

#define BLOCK_SIZE KB(4)

#define FREE 0
#define USED 1
typedef unsigned char state_t;

void* mem_base;
size_t mem_length;

state_t* mem_map;
size_t map_entries;

void* mem_alloc(size_t size)
{
	kernel_panic("Not implemented");
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

void mem_init(size_t memory_size)
{
	// We split the map into enough entries so that the entire page table fits inside it.
	map_entries = (memory_size / BLOCK_SIZE);
	mem_map = page_alloc(map_entries * sizeof(state_t));
	memset(mem_map, 0, map_entries * sizeof(state_t));
}




















