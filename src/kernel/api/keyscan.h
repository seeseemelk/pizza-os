/**
 * Contains keyboard scancode definitions
 */

#ifndef API_KEYSCAN_H_
#define API_KEYSCAN_H_

#include "cdefs.h"

typedef u8 SCANCODE;

typedef enum scanaction_t scanaction_t;
enum scanaction_t
{
	SA_PRESSED,
	SA_RELEASED
};

typedef struct scancode_t scancode_t;
struct scancode_t
{
	SCANCODE code; /* Refers to the specific key */
	scanaction_t action; /* */
};

/*
#define KB_ESC 0x00
#define KB_1 0x01
#define KB_2 0x02
#define KB_3 0x03
#define KB_4 0x04
#define KB_5 0x05
#define KB_6 0x06
#define KB_7 0x07
#define KB_8 0x08
#define KB_9 0x09
#define KB_0 0x0A
#define KB_A 0x0B
#define KB_B 0x0C
#define KB_C 0x0D
#define KB_D 0x0E
#define KB_E 0x0F
#define KB_F 0x10
#define KB_G 0x11
#define KB_H 0x12
#define KB_I 0x13
#define KB_J 0x14
#define KB_K 0x15
#define KB_L 0x16
#define KB_M 0x17
#define KB_N 0x18
#define KB_O 0x19
#define KB_P 0x1A
#define KB_Q 0x1B
#define KB_R 0x1C
#define KB_S 0x1D
#define KB_T 0x1E
#define KB_U 0x1F
#define KB_V 0x20
#define KB_W 0x21
#define KB_X 0x22
#define KB_Y 0x23
#define KB_Z 0x24
*/

#define KB_F9 0x01
#define KB_F5 0x03
#define KB_F3 0x04
#define KB_F1 0x05
#define KB_F2 0x06
#define KB_F12 0x07
#define KB_F10 0x09
#define KB_F8 0x0A
#define KB_F6 0x0B
#define KB_F4 0x0C
#define KB_TAB 0x0D
#define KB_BACKTICK 0x0E
#define KB_LALT 0x11
#define KB_LSHIFT 0x12
#define KB_LCONTROL 0x14
#define KB_Q 0x15
#define KB_1 0x16
#define KB_Z 0x1A
#define KB_S 0x1B


// And so on and so forth
// https://wiki.osdev.org/PS/2_Keyboard

#endif /* API_KEYSCAN_H_ */
