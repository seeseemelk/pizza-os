/*
 * stdio.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "stdio.h"
#include "dev/tty.h"

#include <stdarg.h>
#include <stdbool.h>

static const char radixLowerChars[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
static const char radixUpperChars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static int print_unumber(unsigned long long number, int radix, bool upper)
{
	char converted[64]; // Longest binary number possible
	int size = 0;

	const char* chars = (upper) ? radixUpperChars : radixLowerChars;
	if (number == 0)
	{
		putchar(chars[0]);
		return 1;
	}
	else
	{
		while (number > 0)
		{
			converted[size++] = chars[number % radix];
			number /= radix;
		}

		for (int i = size-1; i >= 0; i--)
		{
			putchar(converted[i]);
		}

		return size;
	}
}

static int print_number(signed long long number, int radix, bool upper)
{
	if (number >= 0)
		return print_unumber(number, radix, upper);
	else
	{
		putchar('-');
		return print_unumber(-number, radix, upper) + 1;
	}
}

int putchar(char c)
{
	tty_put_char(c);
	return c;
}

int puts(const char* str)
{
	tty_print(str);
	return 1;
}

int printf(const char* format, ...)
{
	int length = 0;
	va_list args;
	va_start(args, format);

	bool formatOption = false;
	char c;
	while ((c = *format) != '\0')
	{
		if (formatOption)
		{
			switch (c)
			{
			case '%':
				putchar('%');
				length++;
				break;
			case 'd':
			case 'i':
				length += print_number(va_arg(args, int), 10, false);
				break;
			case 'u':
				length += print_unumber(va_arg(args, int), 10, false);
				break;
			case 'o':
				length += print_unumber(va_arg(args, int), 8, false);
				break;
			case 'x':
				length += print_unumber(va_arg(args, int), 16, false);
				break;
			case 'X':
				length += print_unumber(va_arg(args, int), 16, true);
				break;
			case 'c':
				putchar((char) va_arg(args, int));
				length++;
				break;
			case 's':
				length += puts(va_arg(args, const char*));
				break;
			case 'p':
				length += print_unumber(va_arg(args, int), 16, true);
				break;
			}
			formatOption = false;
		}
		else
		{
			if (c == '%')
				formatOption = true;
			else
			{
				putchar(c);
				length++;
			}
		}

		format++;
	}

	va_end(args);
	return length;
}

