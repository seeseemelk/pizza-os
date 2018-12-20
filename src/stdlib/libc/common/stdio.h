/*
 * stdio.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>

// The k prefix
int kputs(const char* str);
int kputchar(char character);
int kvprintf(const char* format, va_list arg);
int kprintf(const char* format, ...);

#endif /* STDIO_H_ */
