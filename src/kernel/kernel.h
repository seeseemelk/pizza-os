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

/**
 * Logs a message to the kernel.
 * @param fmt A formatted string to log.
 */
void kernel_log(const char* fmt, ...);

/**
 * Gets the kernel time in seconds.
 * @return The time the kernel has been running in whole seconds.
 */
long long kernel_time();

/**
 * Gets the microseconds part of the kernel time.
 * @return The microseconds part of the kernel time.
 */
int kernel_time_us();

/**
 * Adds a number of nanoseconds to the kernel time.
 * @return us The number of nanoseconds to add to the kernel time.
 */
void kernel_time_add(unsigned long long us);

#endif /* KERNEL_H_ */
