/*
 * tty.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef DEV_TTY_H_
#define DEV_TTY_H_
#include "devices.h"

void tty_set_tty(device_t* device);

char tty_get_char(const int x, const int y);
void tty_set_char(const int x, const int y, char c);

void tty_set_cursor(const int x, const int y);

void tty_put_char(char);
void tty_print(const char* str);
void tty_scroll(const int lines);
void tty_clear();

#endif /* DEV_TTY_H_ */
