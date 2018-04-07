/**
 * @file kernel.h
 * Contains basic kernel methods.
 * These do not fit in any other specific header file.
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stddef.h>

/**
 * Causes a kernel panic.
 * Takes a formatted string as extra information to display
 * on the kernel panic screen and then halts the system completely.
 * @param fmt The format string to use.
 */
void kernel_panic(const char* fmt, ...);

#endif /* KERNEL_H_ */
