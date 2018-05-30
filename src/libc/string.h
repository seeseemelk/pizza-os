/*
 * string.h
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

void* memcpy(void* dest, void* src, size_t num);
void* memset(void* ptr, int value, size_t num);
size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t num);
char* strndup(const char* str, size_t len);
char* strdup(const char* str);

#endif /* STRING_H_ */
