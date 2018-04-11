/*
 * tty.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "dev/tty.h"
#include <stddef.h>

device_t* tty;
int width;
int height;
int max_index;

void tty_set_tty(device_t* dev)
{
	tty = dev;
	width = device_invoke(tty, GET_WIDTH);
	height = device_invoke(tty, GET_HEIGHT);
	max_index = width * height;
}

char tty_get_char(const int x, const int y)
{
	return device_invoke2(tty, GET_CHAR, x, y);
}

void tty_set_char(const int x, const int y, char c)
{
	device_invoke3(tty, SET_CHAR, x, y, c);
}

void tty_set_cursor(const int x, const int y)
{
	device_invoke2(tty, SET_CURSOR, x, y);
}

int tty_get_cursor_x()
{
	return device_invoke(tty, GET_CURSOR_X);
}

int tty_get_cursor_y()
{
	return device_invoke(tty, GET_CURSOR_Y);
}

static void tty_advance_line()
{
	int cursor_y = tty_get_cursor_y() + 1;
	if (cursor_y >= height)
	{
		tty_scroll(1);
		tty_set_cursor(0, cursor_y - 1);
	}
	else
	{
		tty_set_cursor(0, cursor_y);
	}
}

static void tty_advance_cursor()
{
	int cursor_x = tty_get_cursor_x() + 1;
	int cursor_y = tty_get_cursor_y();
	if (cursor_x >= width)
		tty_advance_line();
	else
		tty_set_cursor(cursor_x, cursor_y);
}

void tty_put_char(const char c)
{
	if (c == '\n')
	{
		tty_advance_line();
	}
	else
	{
		int cursor_x = tty_get_cursor_x();
		int cursor_y = tty_get_cursor_y();
		tty_set_char(cursor_x, cursor_y, c);
		tty_advance_cursor();
	}
}

void tty_print(const char* str)
{
	while (*str != '\0')
	{
		tty_put_char(*str);
		str++;
	}
}

void tty_scroll(const int lines)
{
	device_invoke1(tty, SCROLL, lines);
}

void tty_clear()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tty_set_char(x, y, ' ');
		}
	}
	tty_set_cursor(0, 0);
}









