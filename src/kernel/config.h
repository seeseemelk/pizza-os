/**
 * @file
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "cdefs.h"

/**
 * The target system to compile for.
 * Can be one of the following: i386
 */
#define TARGET i386

/** The maximum number of modules that can be loaded. */
#define MAX_MODULES 255

/** The maximum number of devices that a module can have. */
#define MAX_DEVICES 255

/** The maximum number of interrupt handlers that can be loaded for each interrupt. */
#define MAX_NUM_INT_HANDLERS 16

/** Enables VGA drivers if defined */
#define ENABLE_VGA

/** The minimum block size that the physical memory allocator can allocate */
#define PMEM_BLOCK_SIZE KB(4)

#endif /* CONFIG_H_ */
