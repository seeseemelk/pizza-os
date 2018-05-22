#include "keymap.h"
#include "thread/signal.h"
#include "keymaps/enUS.h"
#include "kernel.h"
#include <stdbool.h>

static bool scancodes[256];
static signal_t signal;

// Allows buffering of coding.
#define BUF_LENGTH 16
static char buf[BUF_LENGTH];
static int buf_read_index = 0;
static int buf_write_index = 0;
static int buf_size = 0;

void keyboard_init()
{
	signal_new(&signal);
}

u8 keyboard_get_char(SCANCODE code)
{
	return (keymapNormal[code]);
}

void keyboard_register_event(scancode_t scancode)
{
	if (scancode.action == SA_PRESSED)
	{
		scancodes[scancode.code] = true;

		if (buf_size < BUF_LENGTH)
		{
			buf[buf_write_index] = keyboard_get_char(scancode.code);
			buf_write_index = (buf_write_index + 1) % BUF_LENGTH;
			buf_size++;
			signal_signal(&signal);
		}
	}
	else
	{
		scancodes[scancode.code] = false;
	}
}

static void keyboard_wait()
{
	signal_wait(&signal);
}

bool keyboard_is_down(SCANCODE scan)
{
	return scancodes[scan];
}

char keyboard_read_char()
{
	keyboard_wait();
	char c = buf[buf_read_index];
	buf_read_index = (buf_read_index + 1) % BUF_LENGTH;
	buf_size--;
	return c;
}














