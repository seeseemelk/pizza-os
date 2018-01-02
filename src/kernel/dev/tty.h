/*
 * tty.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef DEV_TTY_H_
#define DEV_TTY_H_

typedef struct
{
	int width;
	int height;
	char (*get)(const int i);
	void (*set)(const int i, char c);
	void (*set_cursor)(const int i);
	int (*get_cursor)(void);
	void (*scroll)(const int lines);
} TTY;

void tty_set_tty(const TTY* tty, const int num);
void tty_select(const int num);

char tty_get_char(const int x, const int y);
void tty_set_char(const int x, const int y, char c);

void tty_set_cursor(const int x, const int y);

void tty_put_char(const char);
void tty_print(const char* str);
void tty_scroll(const char* lines);

#endif /* DEV_TTY_H_ */
