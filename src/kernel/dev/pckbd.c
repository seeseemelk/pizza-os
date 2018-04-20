#include <dev/pcps2.h>
#include "pckbd.h"
#include "devices.h"
#include "cdefs.h"

module_t* pckbd;
device_t* keyboard;

int pckbd_req(dev_req_t* req)
{
	UNUSED(req);
	return 0;
}

void pckbd_init()
{
	pckbd = module_register("pckbd", KEYBOARD, NULL, &pckbd_req);
	keyboard = device_register(pckbd);
}
