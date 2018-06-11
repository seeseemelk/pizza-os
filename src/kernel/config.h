/**
 * @file
 * Contains configuration information.
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

/** The maximum number of threads that can be allocated at any one time. */
#define MAX_THREADS 16

/** The minimum block size that the physical memory allocator can allocate */
#define PMEM_BLOCK_SIZE KB(4)

/** The maximum length of a filename in bytes, including the null terminator */
#define MAX_FILENAME 256

/** The maximum length of a path in bytes, including the null terminator */
#define MAX_PATH 4096

/** Selects the required keyboard layout */
//#define KEYBOARD_LAYOUT "api/keymaps/enUS.h"
#define KEYBOARD_LAYOUT "api/keymaps/nlBE.h"

/**
 * How often the scheduler should preempt a thread in milliseconds.
 * Note that it can only be as accurate as the interval of the PIT.
 */
#define SCHED_PREEMPT_INTERVAL 20000

/* These enable specific drivers */
#define ENABLE_VGA
#define ENABLE_TMPFS
#define ENABLE_PS2
#define ENABLE_PS2KBD

#if TARGET == i386
#include "arch/i386/config.h"
#endif

#endif /* CONFIG_H_ */
