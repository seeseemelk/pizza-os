/*
 * kernel.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stddef.h>

size_t memory_available;
void kernel_panic(const char* fmt, ...);

#endif /* KERNEL_H_ */
