/*
 * page.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_PC_PAGE_H_
#define ARCH_PC_PAGE_H_

#include "page.h"
#include "cdefs.h"

#include <stddef.h>
#include <stdbool.h>

typedef u32 page_entry;
typedef page_entry page;

size_t page_get_address(page_entry* entry);
void page_set_address(page_entry* entry, void* address);
void page_set_flags(page_entry* entry, bool present, bool rw, bool user);
void page_init();

#endif /* ARCH_PC_PAGE_H_ */
