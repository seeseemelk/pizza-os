#include "bus/ps2.h"
#include "kernel.h"
#include "pckbd.h"
#include "devices.h"
#include "cdefs.h"
#include <stdlib.h>
#include "../thread/signal.h"

#define DEV(obj) ((device_t*)obj)
#define PS2(obj) ((pskbd_t*)obj)

typedef struct
{
	device_t dev;
	ps2_bus_t* bus;
	signal_t int_signal;
} pckbd_t;

module_t mod;
pckbd_t kbd;

int pckbd_req(dev_req_t* req)
{
	if (req->type == READ)
	{
		/*lock_wait(lock);
		lock_lock(lock)*/
		return 0;
	}
	else
	{
		kernel_panic("Unsupported request");
		return 0;
	}
}

void pckbd_init(ps2_bus_t* bus)
{
	module_register(&mod, "pckbd", KEYBOARD, NULL, &pckbd_req);
	device_register(DEV(&kbd), &mod);

	kbd.bus = bus;
	signal_new(&kbd.int_signal);
}
