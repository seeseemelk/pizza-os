/*
 * devices.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "dev/devices.h"
#include "config.h"

#include <stddef.h>

module modules[MAX_MODULES];
device devices[MAX_DEVICES];
int num_modules_loaded = 0;
int num_devices_loaded = 0;

module* module_register(const char* name, module_type type, fn_module_request* fn_request)
{
	const module* module = &modules[num_modules_loaded];
	module->name = name;
	module->type = type;
	module->fn_request = fn_request;
	module->major = num_modules_loaded;
	module->num_devices_loaded = 0;
	num_modules_loaded++;
	return module;
}

device* device_register(const module* module, const int minor)
{
	const device* device = &devices[num_devices_loaded];
	device->module = module;
	device->minor = module->num_devices_loaded;
	module->num_devices_loaded++;
	return device;
}

/*
 * Functions for finding a specific device.
 */
device* device_get_first(module_type type)
{
	for (int i = 0; i < num_devices_loaded; i++)
	{
		if (devices[i]->module->type == type)
		{
			return devices[i];
		}
	}
	return NULL;
}

/*
 * Functions for communicating with a device.
 */
module_request requests;

int device_invoke4(device* device, request_type type, int arg1, int arg2, int arg3, int arg4)
{
	requests.device = device;
	requests.type = type;
	requests.arg1 = arg1;
	requests.arg2 = arg2;
	requests.arg3 = arg3;
	requests.arg4 = arg4;
	return device->module->fn_request(&requests);
}

int device_invoke3(device* device, request_type type, int arg1, int arg2, int arg3)
{
	return device_invoke(device, type, arg1, arg2, arg3, 0);
}

int device_invoke2(device* device, request_type type, int arg1, int arg2)
{
	return device_invoke(device, type, arg1, arg2, 0, 0);
}

int device_invoke1(device* device, request_type type, int arg1)
{
	return device_invoke(device, type, arg1, 0, 0, 0);
}

int device_invoke(device* device, request_type type)
{
	return device_invoke(device, type, 0, 0, 0, 0);
}

















