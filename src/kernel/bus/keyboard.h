/*
 * keyboard.h
 *
 *  Created on: May 21, 2018
 *      Author: seeseemelk
 */

#ifndef BUS_KEYBOARD_H_
#define BUS_KEYBOARD_H_

#include "api/keyscan.h"

typedef struct keyboard_t keyboard_t;
struct keyboard_t
{
	device_t* device;
	//void(*wait_scancode)(device_t* device, scancode_t* scancode);
};

/*
inline void keyboard_wait_scancode(keyboard_t* kbd, scancode_t* scancode)
{
	kbd->wait_scancode(kbd->device, scancode);
}
*/

#endif /* BUS_KEYBOARD_H_ */
