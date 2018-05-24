/*
 * bus.h
 *
 *  Created on: May 7, 2018
 *      Author: seeseemelk
 */

#ifndef BUS_BUS_H_
#define BUS_BUS_H_

typedef enum bus_t bus_t;

enum bus_t
{
	PS2, VGA, KEYBOARD, FILESYSTEM,
	BUSCOUNT
};

#endif /* BUS_BUS_H_ */
