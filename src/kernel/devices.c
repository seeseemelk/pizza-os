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
	void* bus;
	device_t* dev;
} dev_bus_t;

typedef struct
{
	void* bus;
	module_t* mod;
} mod_bus_t;

module_t* modules[MAX_MODULES];
device_t* devices[MAX_DEVICES];
size_t num_modules_loaded = 0;
size_t num_devices_loaded = 0;

/* Contains all modules that registered busses */
size_t num_mbus_loaded[BUSCOUNT] = {0};
mod_bus_t mbusses[BUSCOUNT][MAX_MODULES];

/* Contains all devices that registered busses */
size_t num_dbus_loaded[BUSCOUNT] = {0};
dev_bus_t dbusses[BUSCOUNT][MAX_DEVICES];

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

void module_register_bus(module_t* module, bus_t type, void* bus)
{
	if (type != VGA && module->fn_dev_req == NULL)
		kernel_log("Module %s registered bus without fn_dev_req", module->name);
	mod_bus_t* mod_bus = mbusses[type] + num_mbus_loaded[type];
	num_mbus_loaded[type]++;
	mod_bus->mod = module;
	mod_bus->bus = bus;
}

void device_register(device_t* device, module_t* module)
{
	devices[num_devices_loaded++] = device;
	device->module = module;
	device->minor = module->num_devices_loaded;
	module->num_devices_loaded++;
	mutex_new(&device->mutex);
}

void device_register_bus(device_t* device, bus_t type, void* bus)
{
	if (type != VGA && device->module->fn_dev_req == NULL)
		kernel_log("Device %s registered bus without fn_dev_req", device->module->name);
	dev_bus_t* dev_bus = dbusses[type] + num_dbus_loaded[type];
	num_dbus_loaded[type]++;
	dev_bus->dev = device;
	dev_bus->bus = bus;
}

/*
 * Functions for finding a module for a specific bus.
 */
void* module_get_first(bus_t type)
{
	size_t loaded = num_mbus_loaded[type];
	if (loaded > 0)
		return mbusses[type][0].bus;
	else
		return NULL;
}

/*
 * Functions for finding a device for a specific bus.
 */
void* device_get_first(bus_t type)
{
	size_t loaded = num_dbus_loaded[type];
	if (loaded > 0)
		return dbusses[type][0].bus;
	else
		return NULL;
}

void* module_get_bus(module_t* mod, bus_t type)
{
	size_t loaded = num_mbus_loaded[type];
	for (size_t i = 0; i < loaded; i++)
	{
		mod_bus_t* bus = mbusses[type] + i;
		if (bus->mod == mod)
			return bus;
	}
	kernel_panic("Bus not found");
	return NULL;
}

void* device_get_bus(device_t* dev, bus_t type)
{
	size_t loaded = num_dbus_loaded[type];
	for (size_t i = 0; i < loaded; i++)
	{
		dev_bus_t* bus = dbusses[type] + i;
		if (bus->dev == dev)
			return bus->bus;
	}
	kernel_panic("Bus not found");
	return NULL;
}

module_t* module_get(const char* name)
{
	for (size_t i = 0; i < num_modules_loaded; i++)
	{
		module_t* module = modules[i];
		if (strcmp(module->name, name) == 0)
			return module;
	}
	return NULL;
}

device_t* device_get_by_minor(MAJOR major, MINOR minor)
{
	for (size_t i = 0; i < num_devices_loaded++; i++)
	{
		device_t* dev = devices[i];
		if (dev->minor == minor && dev->module->major == major)
			return dev;
	}
	return NULL;
}

void module_it_begin(bus_it* bus, bus_t type)
{
	bus->bus = mbusses[type];
	bus->size = &num_mbus_loaded[type];
	bus->index = 0;
}

void device_it_begin(bus_it* bus, bus_t type)
{
	bus->bus = dbusses[type];
	bus->size = &num_dbus_loaded[type];
	bus->index = 0;
}

void* module_it_next(bus_it* bus)
{
	if (bus->index < *bus->size)
		return ((mod_bus_t*)bus->bus)[bus->index++].bus;
	else
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

void device_lock(device_t* device)
{
	mutex_lock(&device->mutex);
}

void device_unlock(device_t* device)
{
	mutex_unlock(&device->mutex);
}

/**
 * Lets a device allocate some physical memory.
 * A pointer to the memory is returned.
 */
void* device_mmap(void* phys, size_t length)
{
	return page_alloc_phys(phys, length);
}















