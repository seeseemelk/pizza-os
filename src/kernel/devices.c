/*
 * devices.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "devices.h"
#include "config.h"
#include "page.h"
#include "kernel.h"
#include <stddef.h>
#include <string.h>

typedef struct
{
	device_t* dev;
	void* bus;
} dev_bus_t;

module_t* modules[MAX_MODULES];
device_t* devices[MAX_DEVICES];
int num_modules_loaded = 0;
int num_devices_loaded = 0;

int num_bus_loaded[BUSCOUNT];
dev_bus_t busses[BUSCOUNT][MAX_DEVICES];

void module_register(module_t* module, const char* name,
		fn_module_request* fn_mod_req, fn_device_request* fn_dev_req)
{
	modules[num_modules_loaded] = module;
	module->name = name;
	module->fn_mod_req = fn_mod_req;
	module->fn_dev_req = fn_dev_req;
	module->major = num_modules_loaded;
	module->num_devices_loaded = 0;
	num_modules_loaded++;
}

void device_register(device_t* device, module_t* module)
{
	devices[num_devices_loaded++] = device;
	device->module = module;
	device->minor = module->num_devices_loaded;
	module->num_devices_loaded++;
}

void device_register_bus(device_t* device, bus_t type, void* bus)
{
	dev_bus_t* dev_bus = busses[type][num_bus_loaded[type]++];
	dev_bus->dev = device;
	dev_bus->bus = bus;
}

/*
 * Functions for finding a device for a specific bus.
 */
device_t* device_get_first(bus_t type)
{
	int loaded = num_bus_loaded[type];
	if (loaded > 0)
		return busses[type][loaded];
	else
		return NULL;
}

module_t* module_get(const char* name)
{
	for (int i = 0; i < num_modules_loaded; i++)
	{
		module_t* module = modules + i;
		if (strcmp(module->name, name) == 0)
			return module;
	}
	return NULL;
}

device_t* device_get_by_minor(unsigned short major, unsigned short minor)
{
	for (int i = 0; i < num_devices_loaded++; i++)
	{
		device_t* dev = devices + i;
		if (dev->minor == minor && dev->module->major == major)
			return dev;
	}
	return NULL;
}

/*
 * Functions for communicating with a module.
 */

int module_invoke4(module_t* module, request_type type, int arg1, int arg2, int arg3, int arg4)
{
	mod_req_t request;
	request.module = module;
	request.type = type;
	request.arg1 = arg1;
	request.arg2 = arg2;
	request.arg3 = arg3;
	request.arg4 = arg4;
	fn_module_request* fn_mod_req = module->fn_mod_req;
	if (fn_mod_req == NULL)
		kernel_panic("Tried to invoke fn_mod_req on non-fn_mod_req driver");
	return fn_mod_req(&request);
}

int module_invoke3(module_t* module, request_type type, int arg1, int arg2, int arg3)
{
	return module_invoke4(module, type, arg1, arg2, arg3, 0);
}

int module_invoke2(module_t* module, request_type type, int arg1, int arg2)
{
	return module_invoke4(module, type, arg1, arg2, 0, 0);
}

int module_invoke1(module_t* module, request_type type, int arg1)
{
	return module_invoke4(module, type, arg1, 0, 0, 0);
}

int module_invoke(module_t* module, request_type type)
{
	return module_invoke4(module, type, 0, 0, 0, 0);
}

/*
 * Functions for communicating with a device.
 */

int device_invoke4(device_t* device, request_type type, int arg1, int arg2, int arg3, int arg4)
{
	dev_req_t request;
	request.device = device;
	request.type = type;
	request.arg1 = arg1;
	request.arg2 = arg2;
	request.arg3 = arg3;
	request.arg4 = arg4;
	fn_device_request* fn_dev_req = device->module->fn_dev_req;
	if (fn_dev_req == NULL)
		kernel_panic("Tried to invoke fn_dev_req on non-fn_dev_req driver");
	return fn_dev_req(&request);
}

int device_invoke3(device_t* device, request_type type, int arg1, int arg2, int arg3)
{
	return device_invoke4(device, type, arg1, arg2, arg3, 0);
}

int device_invoke2(device_t* device, request_type type, int arg1, int arg2)
{
	return device_invoke4(device, type, arg1, arg2, 0, 0);
}

int device_invoke1(device_t* device, request_type type, int arg1)
{
	return device_invoke4(device, type, arg1, 0, 0, 0);
}

int device_invoke(device_t* device, request_type type)
{
	return device_invoke4(device, type, 0, 0, 0, 0);
}

/**
 * Lets a device allocate some physical memory.
 * A pointer to the memory is returned.
 */
void* device_mmap(void* phys, size_t length)
{
	return page_alloc_phys(phys, length);
}















