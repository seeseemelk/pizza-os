/*
 * devices.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef DEV_DEVICES_H_
#define DEV_DEVICES_H_

/**
 * Defines the type of module a certain module is.
 */
typedef enum
{
	BLOCK, TERMINAL, FILESYSTEM
} module_type;

typedef enum
{
	UNLOAD, /* General request types */
	READ, WRITE, FLUSH, GET_SIZE, /* Block request types */
	GET_CHAR, SET_CHAR, GET_WIDTH, GET_HEIGHT, GET_CURSOR, SET_CURSOR /* Terminal types */
} request_type;

/**
 * The function signature of a module request function.
 */
typedef void(fn_module_request)(void);

/**
 * A struct containing information about a module.
 */
typedef struct
{
	const int major; /**< The major number contains the uniquely identifying number for the module. */
	const char* name; /**< The name is a simple string that will be displayed in logs. */
	const module_type type; /**< The type of module this is. */
	const fn_module_request* fn_request; /**< A pointer to a function that will handle all the request for the module. */
	int num_devices_loaded;
} module;

/**
 * A struct containing the information about a specific device.
 */
typedef struct
{
	const module* module; /**< A pointer to the module that handles this device. */
	const int minor; /** The minor number. This number will uniquely specific a specific device. */
} device;

/**
 * Describes a simple request to a module.
 */
typedef struct
{
	device* device;
	request_type type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
} module_request;

/*
 * Functions for registering modules and devices
 */
module* module_register(const char* name, module_type type, fn_module_request* fn_request);
device* device_register(const module* module, const int minor);

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

#endif /* DEV_DEVICES_H_ */













