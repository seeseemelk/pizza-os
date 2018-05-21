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

module_t* mod;
device_t* kbd;
/*
int pckbd_req(dev_req_t* req)
{
	if (req->type == READ)
	{
		//lock_lock();
		return 0;
	}
	else
	{
		kernel_panic("Unsupported request");
		return 0;
	}
}
*/
void pckbd_init(ps2_bus_t* bus)
{
	UNUSED(bus);
	/*mod = module_register("pckbd", KEYBOARD, NULL, &pckbd_req);
	kbd = device_register(mod);

	pckbd_t* pckbd = malloc(sizeof(pckbd_t));
	pckbd->bus = bus;
	lock_create(&pckbd->lock);

	kbd->data = pckbd;*/
}
