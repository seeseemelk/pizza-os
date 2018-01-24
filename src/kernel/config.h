/*
 * config.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "cdefs.h"

#define TARGET i386

#define MAX_MODULES 255
#define MAX_DEVICES 255
#define MAX_NUM_INT_HANDLERS 16 /* The maximum number of interrupt handlers that can be loaded for each interrupt number. */
#define ENABLE_VGA

#define PMEM_BLOCK_SIZE KB(4)

#endif /* CONFIG_H_ */
