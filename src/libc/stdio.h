/*
 * stdio.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>

int puts(const char* str);
int putchar(char character);
int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

#endif /* STDIO_H_ */
