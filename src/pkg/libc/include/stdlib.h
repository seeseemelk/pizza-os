/*
 * stdlib.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef STDLIB_H_
#define STDLIB_H_

#include <stddef.h>

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

#endif /* STDLIB_H_ */
