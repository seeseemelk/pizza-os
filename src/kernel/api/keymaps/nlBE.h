/*
 * nlBE.h
 *
 *  Created on: May 22, 2018
 *      Author: seeseemelk
 */

#ifndef API_KEYMAPS_NLBE_H_
#define API_KEYMAPS_NLBE_H_

#include "../scancodes.h"

// Hexadecimal numbers are taken from Code Page 437
// https://en.wikipedia.org/wiki/Code_page_437
//
// Some glyphs are not in Code Page 437,
// so Code Page 850 glyhps where used instead.
// https://en.wikipedia.org/wiki/Code_page_850

char keymapNormal[256] = {
		[KB_A] = 'q', [KB_B] = 'b', [KB_C] = 'c', [KB_D] = 'd',
		[KB_E] = 'e', [KB_F] = 'f', [KB_G] = 'g', [KB_H] = 'h',
		[KB_I] = 'i', [KB_J] = 'j', [KB_K] = 'k', [KB_L] = 'l',
		[KB_M] = ',', [KB_N] = 'n', [KB_O] = 'o', [KB_P] = 'p',
		[KB_Q] = 'a', [KB_R] = 'r', [KB_S] = 's', [KB_T] = 't',
		[KB_U] = 'u', [KB_V] = 'v', [KB_W] = 'z', [KB_X] = 'x',
		[KB_Y] = 'y', [KB_Z] = 'w', [KB_SEMICOLON] = 'm',

		[KB_1] = '&',
		[KB_2] = 0x82, // 'é'
		[KB_3] = '"',
		[KB_4] = '\'',
		[KB_5] = '(',
		[KB_6] = 0x15, // '§'
		[KB_7] = 0x8A, // 'è'
		[KB_8] = '!',
		[KB_9] = 0x87, // 'ç'
		[KB_0] = 0xA0,  // 'à',

		[KB_BACKTICK] = 0xFD, // '²'
		[KB_EXTRA1] = '<',
		[KB_MINUS] = ')',
		[KB_EQUALS] = '-',
		[KB_LBRACKET] = '^',
		[KB_RBRACKET] = '$',
		[KB_QUOTE] = 0x97, // 'ù'
		[KB_BACKSLASH] = 0xE6, // 'µ',

		[KB_COMMA] = ';',
		[KB_DOT] = ':',
		[KB_SLASH] = '=',

		[KB_KPDIV] = '/', [KB_KPMUL] = '*',
		[KB_KPMINUS] = '-', [KB_KPPLUS] = '+',
		[KB_KP7] = '7', [KB_KP8] = '8', [KB_KP9] = '9',
		[KB_KP4] = '4', [KB_KP5] = '5', [KB_KP6] = '6',
		[KB_KP1] = '1', [KB_KP2] = '2', [KB_KP3] = '3',
		[KB_KP0] = '0',
		[KB_KPDOT] = '.'
};

char keymapShift[256] = {
		[KB_A] = 'Q', [KB_B] = 'B', [KB_C] = 'C', [KB_D] = 'D',
		[KB_E] = 'E', [KB_F] = 'F', [KB_G] = 'G', [KB_H] = 'H',
		[KB_I] = 'I', [KB_J] = 'J', [KB_K] = 'K', [KB_L] = 'L',
		[KB_M] = '?', [KB_N] = 'N', [KB_O] = 'O', [KB_P] = 'P',
		[KB_Q] = 'A', [KB_R] = 'R', [KB_S] = 'S', [KB_T] = 'T',
		[KB_U] = 'U', [KB_V] = 'V', [KB_W] = 'Z', [KB_X] = 'X',
		[KB_Y] = 'Y', [KB_Z] = 'W', [KB_SEMICOLON] = 'M',

		[KB_1] = '1',
		[KB_2] = '2',
		[KB_3] = '3',
		[KB_4] = '4',
		[KB_5] = '5',
		[KB_6] = '6',
		[KB_7] = '7',
		[KB_8] = '8',
		[KB_9] = '9',
		[KB_0] = '0',

		[KB_BACKTICK] = 0xFC, // '³', CP850
		[KB_EXTRA1] = '>',
		[KB_MINUS] = 0xF8, // '°'
		[KB_EQUALS] = '_',
		[KB_LBRACKET] = 0xF9, // '¨'
		[KB_RBRACKET] = '*',
		[KB_QUOTE] = '%',
		[KB_BACKSLASH] = 0x9C, // '£'

		[KB_COMMA] = '.',
		[KB_DOT] = '/',
		[KB_SLASH] = '+',
};

char keymapAlt[256] = {
		[KB_A] = 'q', [KB_B] = 'b', [KB_C] = 'c', [KB_D] = 'd',
		[KB_E] = 'e', [KB_F] = 'f', [KB_G] = 'g', [KB_H] = 'h',
		[KB_I] = 'i', [KB_J] = 'j', [KB_K] = 'k', [KB_L] = 'l',
		[KB_M] = ',', [KB_N] = 'n', [KB_O] = 'o', [KB_P] = 'p',
		[KB_Q] = 'a', [KB_R] = 'r', [KB_S] = 's', [KB_T] = 't',
		[KB_U] = 'u', [KB_V] = 'v', [KB_W] = 'z', [KB_X] = 'x',
		[KB_Y] = 'y', [KB_Z] = 'w', [KB_SEMICOLON] = 'm',

		[KB_1] = '|',
		[KB_2] = '@',
		[KB_3] = '#',
		[KB_4] = 0xAC, // '¼'
		[KB_5] = 0xAB, // '½'
		[KB_6] = '^',
		[KB_7] = '{',
		[KB_8] = '[',
		[KB_9] = '{',
		[KB_0] = '}',

		[KB_BACKTICK] = 0xAA, // '¬'
		[KB_EXTRA1] = '\\',
		[KB_MINUS] = '\\',
		[KB_EQUALS] = '-',
		[KB_LBRACKET] = '[',
		[KB_RBRACKET] = ']',
		[KB_QUOTE] = '\'',
		[KB_BACKSLASH] = '`',

		[KB_COMMA] = ';',
		[KB_DOT] = 0xF9, // '·'
		[KB_SLASH] = '~',
};

#endif /* API_KEYMAPS_NLBE_H_ */
