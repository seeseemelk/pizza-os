#include "keyboard.h"

#include "thread/signal.h"
#include "thread/mutex.h"
#include "kernel.h"
#include "config.h"
#include "bus/filesystem.h"
#include "fstypes.h"
#include "vfs.h"
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

u8 keyboard_get_char(SCANCODE code)
{
	if (keyboard_is_down(KB_LSHIFT) || keyboard_is_down(KB_RSHIFT))
		return keymapShift[code];
	else if (keyboard_is_down(KB_RALT) || keyboard_is_down(KB_LALT))
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

void* kbf_open(device_t* dev)
{
	UNUSED(dev);
	return NULL;
}

void kbf_close(device_t* dev, void* fd)
{
	UNUSED(dev);
	UNUSED(fd);
}

size_t kbf_write(device_t* dev, void* fd, const char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	UNUSED(buf);
	UNUSED(buf_len);
	kernel_log("Attempt to write to system keyboard");
	return 0;
}

size_t kbf_read(device_t* dev, void* fd, char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	for (size_t i = 0; i < buf_len; i++)
	{
		buf[i] = keyboard_read_char();
	}
	return buf_len;
}

static module_t mod;
static device_t dev;
static fileop_t fop = {
	.dev = &dev,
	.open = kbf_open,
	.close = kbf_close,
	.read = kbf_read,
	.write = kbf_write
};

void keyboard_init()
{
	signal_new(&signal);
	mutex_new(&buf_lock);
	module_register(&mod, "system_keyboard", NULL, NULL);
	device_register(&dev, &mod);
	device_register_bus(&dev, FILEOP, &fop);
	vfs_mkchar("/dev/kbd", &dev);
}

