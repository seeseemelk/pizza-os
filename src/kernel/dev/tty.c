/*
 * tty.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "dev/tty.h"
#include <stddef.h>

TTY* ttys[5] = { NULL };
size_t current_tty = 0;

void tty_set_tty(const TTY* tty, const int num)
{
	ttys[num] = tty;
}

void tty_select(const int num)
{
	current_tty = num;
}

char tty_get_char(const int x, const int y)
{
	return ttys[current_tty]->get(x, y);
}

void tty_set_char(const int x, const int y, char c)
{
	ttys[current_tty]->set(x, y, c);
}

void tty_set_cursor(const int x, const int y)
{
	TTY* tty = ttys[current_tty];
	tty->set_cursor(x + tty->width * y);
}

void tty_put_char(const char c)
{
	TTY* tty = ttys[current_tty];
	size_t index = tty->get_cursor();
	size_t max_index = tty->width * tty->height;
	if (index >= max_index)
		tty_scroll(1);
	tty->set(index + 1, c);
	tty->set_cursor(index + 1);
}

void tty_print(const char* str)
{
	while (*str != '\0')
	{
		tty_put_char(*str);
		str++;
	}
}

void tty_scroll(const char lines)
{
	ttys[current_tty]->scroll(lines);
}



