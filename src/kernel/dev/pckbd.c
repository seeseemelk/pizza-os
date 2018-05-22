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
	bool waiting;
	signal_t signal;
	u8 scancode;
} pckbd_t;

static module_t mod;
static pckbd_t kbd;

void pckbd_wait_scancode(device_t* dev, scancode_t* scancode)
{
	UNUSED(scancode);
	KBD(dev)->waiting = true;
	signal_wait(&KBD(dev)->signal);
	KBD(dev)->scancode = ps2_read_data(KBD(dev)->bus);
}

void pckbd_parse_keycode(pckbd_t* dev)
{
	scancode_t scancode = {
		.action = SA_PRESSED
	};
	bool finished = false;
	while (!finished)
	{
		u8 data = ps2_read_data(dev->bus);
		if (data == 0xF0)
			scancode.action = SA_RELEASED;
		else
		{
			scancode.code = data;
			finished = true;
		}
	}
	UNUSED(scancode);
}

void pckbd_send_packet(pckbd_t* kbd, u8* commands, int length)
{
	bool finished = false;

	while (!finished)
	{
		kbd->waiting = true;
		signal_clear(&kbd->signal);
		for (int i = 0; i < length; i++)
		{
			ps2_write_data(kbd->bus, commands[i]);
		}
		signal_wait(&kbd->signal);

		u8 status;
		while ((status = ps2_read_status(kbd->bus)) != 0xFA)
		{
			// This needs to be improved
			kernel_panic("Keyboard returned status 0x%X", status);
		}
	}
}

int pckbd_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		pckbd_t* kbd = KBD(req->device);
		/*if (kbd->waiting)
		{
			kbd->waiting = false;*/
			signal_signal(&kbd->signal);
			return INT_ACCEPT;
		/*}
		else
			return INT_IGNORE;*/
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
		scancode_t scancode;
		pckbd_wait_scancode(DEV(&kbd), &scancode);
		kernel_log("Found 0x%X", kbd.scancode);
	}
}

void pckbd_init(ps2_bus_t* bus)
{
	module_register(&mod, "pckbd", NULL, &pckbd_req);
	device_register(&kbd.dev, &mod);
	device_register_bus(&kbd.dev, KEYBOARD, &kbd.kbd);
	interrupt_register(&kbd.dev, 0x21);
	kbd.bus = bus;
	kbd.kbd.wait_scancode = pckbd_wait_scancode;
	kbd.waiting = false;
	signal_new(&kbd.signal);

	kernel_log("A");
	signal_clear(&kbd.signal);
	ps2_write_data(bus, 0xFF);
	// Can't wait for interrupt here. Call reset code in pcps2.c

	kernel_log("B");
	ps2_write_data(bus, 0xF0);
	ps2_write_data(bus, 2);

	kernel_log("C");
	ps2_write_data(bus, 0xF4);
	/*for (int i = 0; i < 10; i++)
		ps2_read_data(kbd.bus);*/
	u8 status;
	while (((status = inb(0x64)) & 1) == 1)
		inb(0x60);

	kernel_log("D");
	thread_create(&pckbd_test);
}

















