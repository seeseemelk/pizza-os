/**
 * @file
 * api/keymap.h allows for translation of scan codes received
 * by a keyboard to be mapped to actual characters.
 */

#ifndef API_KEYBOARD_H_
#define API_KEYBOARD_H_

#include <stdbool.h>
#include "scancodes.h"

void keyboard_init();
void keyboard_register_event(scancode_t scancode);
bool keyboard_is_down(SCANCODE scan);
char keyboard_read_char();

#endif /* API_KEYBOARD_H_ */
