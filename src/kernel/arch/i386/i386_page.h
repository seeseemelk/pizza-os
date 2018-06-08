/*
 * page.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_PAGE_H_
#define ARCH_I386_I386_PAGE_H_

#include "i386_page.h"

#include "cdefs.h"

#include <stddef.h>
#include <stdbool.h>

#define PAGE_PRESENT	0b001
#define PAGE_RW			0b010
#define PAGE_USR		0b100

typedef u32 page_entry;
typedef page_entry page;
typedef int page_flags;

/*size_t page_get_address(page_entry* entry);
void page_set_address(page_entry* entry, void* address);
void page_set_flags(page_entry* entry, bool present, bool rw, bool user);*/
void page_init();
void page_enable();

#endif /* ARCH_I386_I386_PAGE_H_ */
