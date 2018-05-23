#include "keyboard.h"

#include "thread/signal.h"
#include "thread/mutex.h"
#include "kernel.h"
#include "config.h"
#include <stdbool.h>

#include KEYBOARD_LAYOUT

static bool scancodes[256];
static signal_t signal;

// Allows buffering of coding.
#define BUF_LENGTH 16
static volatile char buf[BUF_LENGTH];
static int buf_read_index = 0;
static int buf_write_index = 0;
static volatile int buf_size = 0;
static mutex_t buf_lock;

void keyboard_init()
{
	signal_new(&signal);
	mutex_new(&buf_lock);
}

u8 keyboard_get_char(SCANCODE code)
{
	if (keyboard_is_down(KB_LSHIFT) || keyboard_is_down(KB_RSHIFT))
		return keymapShift[code];
	else if (keyboard_is_down(KB_RALT))
		return keymapAlt[code];
	else
		return keymapNormal[code];
}

void keyboard_register_event(scancode_t scancode)
{
	if (scancode.action == SA_PRESSED)
	{
		scancodes[scancode.code] = true;
		// This line can be uncommented when building keymaps
		//kernel_log("Scancode: 0x%X", scancode.code);

		mutex_lock(&buf_lock);
		if (buf_size < BUF_LENGTH)
		{
			buf[buf_write_index] = keyboard_get_char(scancode.code);
			if (buf[buf_write_index] != 0)
			{
				buf_write_index = (buf_write_index + 1) % BUF_LENGTH;
				buf_size++;
				signal_signal(&signal);
			}
		}
		mutex_unlock(&buf_lock);
	}
	else
	{
		scancodes[scancode.code] = false;
	}
}

static void keyboard_wait()
{
	if (buf_size == 0)
		signal_wait(&signal);
}

bool keyboard_is_down(SCANCODE scan)
{
	return scancodes[scan];
}

char keyboard_read_char()
{
	keyboard_wait();
	mutex_lock(&buf_lock);
	char c = buf[buf_read_index];
	buf_read_index = (buf_read_index + 1) % BUF_LENGTH;
	buf_size--;
	mutex_unlock(&buf_lock);
	return c;
}














