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
typedef struct mod_req_t mod_req_t;
typedef struct dev_req_t dev_req_t;
typedef struct module_t module_t;
typedef struct device_t device_t;

/**
 * The function signature of a module request function.
 */
typedef int(fn_module_request)(struct mod_req_t*);

/**
 * The function signature of a device request function.
 */
typedef int(fn_device_request)(struct dev_req_t*);

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
	UNLOAD, INTERRUPT, /* General request types */
	PAGING_ENABLED, /* Called when paging gets enabled */
	READ, WRITE, FLUSH, GET_SIZE, /* Block request types */
	GET_CHAR, SET_CHAR, GET_WIDTH, GET_HEIGHT, GET_CURSOR_X, GET_CURSOR_Y, SET_CURSOR, SCROLL /* Terminal types */
};

/**
 * A struct containing information about a module.
 */
struct module_t
{
	int major; /**< The major number contains the uniquely identifying number for the module. */
	const char* name; /**< The name is a simple string that will be displayed in logs. */
	module_type type; /**< The type of module this is. */
	fn_module_request* fn_mod_req; /**< A pointer to a function that will handle all the request for the module. */
	fn_device_request* fn_dev_req; /**< A pointer to a function that will handle all the request for devices owned by the module. */
	int num_devices_loaded;
};

/**
 * A struct containing the information about a specific device.
 */
struct device_t
{
	const module_t* module; /**< A pointer to the module that handles this device. */
	int minor; /** The minor number. This number will uniquely specific a specific device. */
};

/**
 * Describes a simple request to a device.
 */
struct dev_req_t
{
	const device_t* device;
	request_type type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
};

/**
 * Describes a simple request to a module.
 */
struct mod_req_t
{
	const module_t* module;
	request_type type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
};

/*
 * Functions for registering modules and devices
 */

/**
 * Registers a module.
 * A module has a common name, a type, and has to specify to functions
 * that will be used to handle module- and device requests.
 * Either one of these can be `NULL` if module requests or device
 * requests arent't needed.
 */
module_t* module_register(const char* name, module_type type,
		fn_module_request* fn_mod_req, fn_device_request* fn_dev_req);

/**
 * Registers a new device.
 */
device_t* device_register(module_t* module);

/*
 * Functions for finding a specific device.
 */
device_t* device_get_first(module_type type);

/*
 * Functions for communicating with a module.
 */
int module_invoke4(module_t* module, request_type type, int arg1, int arg2, int arg3, int arg4);
int module_invoke3(module_t* module, request_type type, int arg1, int arg2, int arg3);
int module_invoke2(module_t* module, request_type type, int arg1, int arg2);
int module_invoke1(module_t* module, request_type type, int arg1);
int module_invoke(module_t* module, request_type type);

/*
 * Functions for communicating with a device.
 */
int device_invoke4(device_t* device, request_type type, int arg1, int arg2, int arg3, int arg4);
int device_invoke3(device_t* device, request_type type, int arg1, int arg2, int arg3);
int device_invoke2(device_t* device, request_type type, int arg1, int arg2);
int device_invoke1(device_t* device, request_type type, int arg1);
int device_invoke(device_t* device, request_type type);

/*
 * Functions for allocating and deallocating memory.
 */
void* device_mmap(void* phys, size_t length);

#endif /* DEV_DEVICES_H_ */













