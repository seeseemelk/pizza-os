/*
 * devices.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef DEV_DEVICES_H_
#define DEV_DEVICES_H_
#include <stddef.h>

typedef enum module_type module_type;
typedef enum request_type request_type;
typedef struct module_request module_request;
typedef struct module module;
typedef struct device device;

/**
 * The function signature of a module request function.
 */
typedef int(fn_module_request)(struct module_request*);

/**
 * Defines the type of module a certain module is.
 */
enum module_type
{
	BLOCK, TERMINAL, FILESYSTEM
};

/**
 * Defines the different types of requests that can be made to a module.
 */
enum request_type
{
	UNLOAD, PAGING_ENABLED /* Called when paging gets enabled */, /* General request types */
	READ, WRITE, FLUSH, GET_SIZE, /* Block request types */
	GET_CHAR, SET_CHAR, GET_WIDTH, GET_HEIGHT, GET_CURSOR_X, GET_CURSOR_Y, SET_CURSOR, SCROLL /* Terminal types */
};

/**
 * A struct containing information about a module.
 */
struct module
{
	int major; /**< The major number contains the uniquely identifying number for the module. */
	const char* name; /**< The name is a simple string that will be displayed in logs. */
	module_type type; /**< The type of module this is. */
	fn_module_request* fn_request; /**< A pointer to a function that will handle all the request for the module. */
	int num_devices_loaded;
};

/**
 * A struct containing the information about a specific device.
 */
struct device
{
	const module* module; /**< A pointer to the module that handles this device. */
	int minor; /** The minor number. This number will uniquely specific a specific device. */
};

/**
 * Describes a simple request to a module.
 */
struct module_request
{
	const device* device;
	request_type type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
};

/*
 * Functions for registering modules and devices
 */
module* module_register(const char* name, module_type type, fn_module_request* fn_request);
device* device_register(module* module, const int minor);

/*
 * Functions for finding a specific device.
 */
device* device_get_first(module_type type);

/*
 * Functions for communicating with a device.
 */
int device_invoke4(device* device, request_type type, int arg1, int arg2, int arg3, int arg4);
int device_invoke3(device* device, request_type type, int arg1, int arg2, int arg3);
int device_invoke2(device* device, request_type type, int arg1, int arg2);
int device_invoke1(device* device, request_type type, int arg1);
int device_invoke(device* device, request_type type);

/*
 * Functions for allocating and deallocating memory.
 */
void* device_mmap(void* phys, size_t length);

#endif /* DEV_DEVICES_H_ */













