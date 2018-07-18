#include "dev/tty.h"
#include "dev/eserial.h"
#include "bus/vga.h"
#include <stddef.h>

vga_bus_t* tty = NULL;
int width;
int height;
int max_index;

void tty_set_tty(vga_bus_t* dev)
{
	tty = dev;
	vga_get_size(dev, &width, &height);
	max_index = width * height;
}

char tty_get_char(const int x, const int y)
{
	return vga_get_char(tty, x, y);
}

void tty_set_char(const int x, const int y, char c)
{
	vga_set_char(tty, x, y, c);
}

void tty_set_cursor(const int x, const int y)
{
	vga_set_cursor(tty, x, y);
}

int tty_get_cursor_x()
{
	int x, y;
	vga_get_cursor(tty, &x, &y);
	return x;
}

int tty_get_cursor_y()
{
	int x, y;
	vga_get_cursor(tty, &x, &y);
	return y;
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
	int cursor_x, cursor_y;
	vga_get_cursor(tty, &cursor_x, &cursor_y);

	if (cursor_x >= width)
		tty_advance_line();
	else
		tty_set_cursor(cursor_x+1, cursor_y);
}

void tty_put_char(const char c)
{
	if (c == '\n')
	{
		eserial_putchar('\n');
		eserial_putchar('\r'); // Needed for cu
		if (tty != NULL)
			tty_advance_line();
	}
	else
	{
		eserial_putchar(c);
		if (tty != NULL)
		{
			int cursor_x = tty_get_cursor_x();
			int cursor_y = tty_get_cursor_y();
			tty_set_char(cursor_x, cursor_y, c);
			tty_advance_cursor();
		}
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
	vga_scroll(tty, lines);
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









