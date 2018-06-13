/**
 * @file
 * Contains device management related functions.
 */

#ifndef DEV_DEVICES_H_
#define DEV_DEVICES_H_
#include "bus.h"
#include "thread/mutex.h"
#include <stddef.h>

typedef enum module_type module_type;
typedef enum request_type request_type;
typedef struct mod_req_t mod_req_t;
typedef struct dev_req_t dev_req_t;
typedef struct module_t module_t;
typedef struct device_t device_t;
typedef struct bus_it bus_it;
typedef unsigned char MAJOR;
typedef unsigned char MINOR;

/**
 * The function signature of a module request function.
 */
typedef int(fn_module_request)(struct mod_req_t*);

/**
 * The function signature of a device request function.
 */
typedef int(fn_device_request)(struct dev_req_t*);

/**
 * Defines the different types of requests that can be made to a module.
 */
enum request_type
{
	UNLOAD, INTERRUPT, /* General request types */
	PAGING_ENABLED, /* Called when paging gets enabled */
};

/**
 * A struct containing information about a module.
 */
struct module_t
{
	MAJOR major; /**< The major number contains the uniquely identifying number for the module. */
	const char* name; /**< The name is a simple string that will be displayed in logs. */
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
	MINOR minor; /**< The minor number. This number will uniquely specific a specific device. */
	mutex_t mutex; /**< This mutex provides the ability for software to lock the device. */
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

/**
 * An iterator that iterates over registered busses.
 */
struct bus_it
{
	void* bus;
	size_t* size;
	size_t index;
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
void module_register(module_t* module, const char* name, fn_module_request* fn_mod_req, fn_device_request* fn_dev_req);

/**
 * Registers a module as a bus.
 */
void module_register_bus(module_t*, bus_t type, void* bus);

/**
 * Registers a new device.
 */
void device_register(device_t* device, module_t* module);

/**
 * Registers a device as a bus.
 */
void device_register_bus(device_t*, bus_t type, void* bus);

/*
 * Functions for finding a specific module.
 */
void* module_get_first(bus_t type);

/*
 * Functions for finding a specific device.
 */
void* device_get_first(bus_t type);

/**
 * Finds a specific registered bus of a module.
 */
void* module_get_bus(module_t* mod, bus_t type);

/**
 * Finds a specific registered bus of a device.
 */
void* device_get_bus(device_t* dev, bus_t type);

/**
 * Gets a module by its name.
 * @param name The name of the module to get.
 * @return The module or `NULL` if no such module exists.
 */
module_t* module_get(const char* name);

/**
 * Gets a device by its mayor and minor number.
 * @param major The major number of the owning module.
 * @param minor The minor number of the device.
 * @return The device, or `null` if there is no device with the given major/minor number.
 */
device_t* device_get_by_minor(MAJOR major, MINOR minor);

void module_it_begin(bus_it* bus, bus_t type);
void device_it_begin(bus_it* bus, bus_t type);
void* module_it_next(bus_it* bus);
#define device_it_next module_it_next

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

/**
 * Locks the device.
 * This functions behaves exactly as a mutex.
 * @param device The device to lock.
 */
void device_lock(device_t* device);

/**
 * Unlocks the device.
 * This functions behaves exactly as a mutex.
 * @param device The device to unlock.
 */
void device_unlock(device_t* device);

/*
 * Functions for allocating and deallocating memory.
 */
//void* device_mmap(void* phys, size_t length);

#endif /* DEV_DEVICES_H_ */













