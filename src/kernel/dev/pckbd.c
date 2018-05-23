#include "bus/ps2.h"
#include "bus/keyboard.h"
#include "api/keymap.h"
#include "cdefs.h"
#include "devices.h"
#include "interrupt.h"
#include "kernel.h"
#include "pckbd.h"
#include "threads.h"
#include "thread/signal.h"
#include "io.h"
#include <stdlib.h>

#define DEV(obj) ((device_t*)obj)
#define KBD(obj) ((pckbd_t*)obj)

typedef struct
{
	device_t dev;
	keyboard_t kbd;
	ps2_bus_t* bus;
	enum {
		S_NEWKEY,
		S_EXTRAKEY,
	} state;
	bool enabled;
	bool accept_ints;
	scancode_t scancode;
} pckbd_t;

static module_t mod;
static pckbd_t kbd = {
	.enabled = false,
	.accept_ints = false
};

void pckbd_reset_state(pckbd_t* kbd)
{
	kbd->scancode.code = 0;
	kbd->scancode.action = SA_PRESSED;
	kbd->state = S_NEWKEY;
}

void pckbd_state(pckbd_t* kbd)
{
	if (kbd->state == S_NEWKEY)
	{
		u8 data = ps2_read_data(kbd->bus);
		if (data == 0xF0)
		{
			kbd->scancode.action = SA_RELEASED;
			kbd->state = S_EXTRAKEY;
		}
		else if (data == 0xE0)
		{
			kbd->scancode.code = 0x80;
			kbd->state = S_EXTRAKEY;
		}
		else
		{
			kbd->scancode.code = data;
			keyboard_register_event(kbd->scancode);
			pckbd_reset_state(kbd);
		}
	}
	else if (kbd->state == S_EXTRAKEY)
	{
		u8 data = ps2_read_data(kbd->bus);
		if (data == 0xF0)
		{
			kbd->scancode.action = SA_RELEASED;
			kbd->state = S_EXTRAKEY;
		}
		else if (data == 0xE0)
		{
			kbd->scancode.code = 0x80;
			kbd->state = S_EXTRAKEY;
		}
		else
		{
			kbd->scancode.code |= data;
			keyboard_register_event(kbd->scancode);
			pckbd_reset_state(kbd);
		}
	}
}

int pckbd_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		pckbd_t* kbd = KBD(req->device);
		if (kbd->enabled)
		{
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

void pckbd_init(ps2_bus_t* bus)
{
	module_register(&mod, "pckbd", NULL, &pckbd_req);
	device_register(&kbd.dev, &mod);
	device_register_bus(&kbd.dev, KEYBOARD, &kbd.kbd);
	interrupt_register(&kbd.dev, 0x21);
	kbd.bus = bus;
	pckbd_reset_state(&kbd);

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
}

















