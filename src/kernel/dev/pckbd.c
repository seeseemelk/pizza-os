#include "bus/ps2.h"
#include "bus/filesystem.h"
#include "vfs.h"
#include "cdefs.h"
#include "devices.h"
#include "interrupt.h"
#include "kernel.h"
#include "pckbd.h"
#include "threads.h"
#include "thread/signal.h"
#include "io.h"
#include "../api/keyboard.h"
#include <stdlib.h>

#define DEV(obj) ((device_t*)obj)
#define KBD(obj) ((pckbd_t*)obj)

typedef struct
{
	device_t dev;
	ps2_bus_t* bus;
	bool enabled;
	bool accept_ints;
	scancode_t scancode;
} pckbd_t;

static module_t mod;
static pckbd_t kbd = {
	.enabled = false,
	.accept_ints = false,
	.scancode = {
		.code = 0,
		.action = SA_PRESSED
	}
};

void pckbd_state(pckbd_t* kbd)
{
	u8 data = ps2_read_data(kbd->bus);
	if (data == 0x00 || data == 0xFF)
		return; // Key detect error

	if (data == 0xF0)
	{
		kbd->scancode.action = SA_RELEASED;
	}
	else if (data == 0xE0)
	{
		kbd->scancode.code = 0x80;
	}
	else
	{
		kbd->scancode.code = data;
		keyboard_register_event(kbd->scancode);

		// Reset the state
		kbd->scancode.code = 0;
		kbd->scancode.action = SA_PRESSED;
	}
}

int pckbd_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		pckbd_t* kbd = KBD(req->device);
		if (kbd->enabled)
		{
			u8 status;
			while (((status = ps2_read_status(kbd->bus)) & 1) == 1)
				pckbd_state(kbd);
			return INT_ACCEPT;
		}
		else
		{
			return kbd->accept_ints ? INT_ACCEPT : INT_IGNORE;
		}
	}
	else
	{
		kernel_panic("Unsupported request");
		return 0;
	}
}

void pckbd_test()
{
	kernel_log("Waiting for scancodes");
	while (1)
	{
		char c = keyboard_read_char();
		kernel_log("Found '%c'", c);
	}
}

static fileop_t fopt;

void* pckbd_open(device_t* dev)
{
	UNUSED(dev);
	kernel_log("Opened");
	return NULL;
}

void pckbd_close(device_t* dev, void* data)
{
	UNUSED(dev);
	UNUSED(data);
}

size_t pckbd_read(device_t* dev, void* data, char* buf, size_t len)
{
	UNUSED(dev);
	UNUSED(data);

	const char* str = "Hello";
	len = (len < 5) ? len : 5;
	memcpy(buf, str, len);
	return len;
}

void pckbd_init(ps2_bus_t* bus)
{
	module_register(&mod, "pckbd", NULL, &pckbd_req);
	device_register(&kbd.dev, &mod);
	interrupt_register(&kbd.dev, 0x21);
	kbd.bus = bus;

	ps2_write_data(bus, 0xFF);
	// Can't wait for interrupt here. Call reset code in pcps2.c

	ps2_write_data(bus, 0xF0);
	ps2_write_data(bus, 2);

	ps2_write_data(bus, 0xF4);

	kbd.accept_ints = true;
	u8 status;
	while (((status = ps2_read_status(bus)) & 1) == 1)
		ps2_read_data(bus);
	kbd.accept_ints = false;

	kbd.enabled = true;
	thread_create(&pckbd_test);

	fopt.dev = &kbd.dev;
	fopt.open = pckbd_open;
	fopt.close = pckbd_close;
	fopt.read = pckbd_read;
	device_register_bus(&kbd.dev, FILEOP, &fopt);
	vfs_mkchar("/dev/kbd", &kbd.dev);
}

















