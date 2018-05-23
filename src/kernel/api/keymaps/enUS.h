/*
 * enUS.h
 *
 *  Created on: May 22, 2018
 *      Author: seeseemelk
 */

#ifndef API_KEYMAPS_ENUS_H_
#define API_KEYMAPS_ENUS_H_

#include "api/keyscan.h"

char keymapNormal[256] = {
		[KB_Q] = 'q'
};

char keymapShift[256] = {};

char keymapAlt[256] = {};

/*
static char keymapNormal[256] = {
	0  , '1', '2', '3', '4', '5', '6', '7',
	'8', '9', '0', 'a', 'b', 'c', 'd', 'e',
	'f', 'g', 'h', 'i',	'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z'
};
*/

#endif /* API_KEYMAPS_ENUS_H_ */
