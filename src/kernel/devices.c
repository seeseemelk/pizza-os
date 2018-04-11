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

module_t modules[MAX_MODULES];
device_t devices[MAX_DEVICES];
int num_modules_loaded = 0;
int num_devices_loaded = 0;

module_t* module_register(const char* name, module_type type,
		fn_module_request* fn_mod_req, fn_device_request* fn_dev_req)
{
	module_t* module = modules + num_modules_loaded;
	module->name = name;
	module->type = type;
	module->fn_mod_req = fn_mod_req;
	module->fn_dev_req = fn_dev_req;
	module->major = num_modules_loaded;
	module->num_devices_loaded = 0;
	num_modules_loaded++;
	return module;
}

device_t* device_register(module_t* module)
{
	device_t* device = devices + num_devices_loaded;
	device->module = module;
	device->minor = module->num_devices_loaded;
	module->num_devices_loaded++;
	return device;
}

/*
 * Functions for finding a specific device.
 */
device_t* device_get_first(module_type type)
{
	for (int i = 0; i < num_devices_loaded; i++)
	{
		if (devices[i].module->type == type)
		{
			return &devices[i];
		}
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















