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


#define KB_F1 0x05
#define KB_F2 0x06
#define KB_F3 0x04
#define KB_F4 0x0C
#define KB_F5 0x03
#define KB_F6 0x0B
#define KB_F7 0x83
#define KB_F8 0x0A
#define KB_F9 0x01
#define KB_F10 0x09
#define KB_F11 0x78
#define KB_F12 0x07

#define KB_A 0x1C
#define KB_B 0x32
#define KB_C 0x21
#define KB_D 0x23
#define KB_E 0x24
#define KB_F 0x2B
#define KB_G 0x34
#define KB_H 0x33
#define KB_I 0x43
#define KB_J 0x3B
#define KB_K 0x42
#define KB_L 0x4B
#define KB_M 0x3A
#define KB_N 0x31
#define KB_O 0x44
#define KB_P 0x4D
#define KB_Q 0x15
#define KB_R 0x2D
#define KB_S 0x1B
#define KB_T 0x2C
#define KB_U 0x3C
#define KB_V 0x2A
#define KB_W 0x1D
#define KB_X 0x22
#define KB_Y 0x35
#define KB_Z 0x1A

#define KB_0 0x45
#define KB_1 0x16
#define KB_2 0x1E
#define KB_3 0x26
#define KB_4 0x25
#define KB_5 0x2E
#define KB_6 0x36
#define KB_7 0x3D
#define KB_8 0x3E
#define KB_9 0x46

#define KB_NUMLOCK 0x77
#define KB_KP0 0x70
#define KB_KP1 0x69
#define KB_KP2 0x72
#define KB_KP3 0x7A
#define KB_KP4 0x6B
#define KB_KP5 0x73
#define KB_KP6 0x74
#define KB_KP7 0x6C
#define KB_KP8 0x75
#define KB_KP9 0x7D
#define KB_KPDOT 0x71
#define KB_KPPLUS 0x79
#define KB_KPMINUS 0x7B
#define KB_KPMUL 0x7C
#define KB_KPDIV 0xCA

#define KB_TAB 0x0D
#define KB_BACKTICK 0x0E
#define KB_LALT 0x11
#define KB_LSHIFT 0x12
#define KB_LCONTROL 0x14
#define KB_SPACE 0x29
#define KB_COMMA 0x41
#define KB_DOT 0x49
#define KB_SLASH 0x4A
#define KB_SEMICOLON 0x4C
#define KB_MINUS 0x4E
#define KB_QUOTE 0x52
#define KB_LBRACKET 0x54
#define KB_RBRACKET 0x5B
#define KB_EQUALS 0x55
#define KB_CAPSLOCK 0x58
#define KB_RSHIFT 0x59
#define KB_ENTER 0x5A
#define KB_BACKSLASH 0x5D
#define KB_BACKSPACE 0x66
#define KB_KPESCAPE 0x76
#define KB_SCROLLOCK 0x7E

/* Multimedia keys (normal scan code BOR 0x80 */
#define KB_WWSEARCH 0x90
#define KB_RALT 0x91
#define KB_RCONTROL 0x94
#define KB_MMPREVTRACK 0x95
#define KB_WWWFAV 0x98
#define KB_LGUI 0x9F
#define KB_WWWREFRESH 0xA0
#define KB_MMVOLDOWN 0xA1
#define KB_MMMUTE 0xA3
#define KB_RGUI 0xA7
#define KB_WWWSTOP 0xA8
#define KB_MMCALC 0xAB
#define KB_APPS 0xAF
#define KB_WWWFORWARD 0xB0
#define KB_MMVOLUP 0xB2
#define KB_MMPLAY 0xB4
#define KB_ACPIPOWER 0xB7
#define KB_WWWBACK 0xB8
#define KB_WWWHOME 0xBA
#define KB_MMSTOP 0xBB
#define KB_ACPISLEEP 0xBF
#define KB_MMCOMPUTER 0xC0
#define KB_MMEMAIL 0xC8
#define KB_MMNEXT 0xCD
#define KB_MMSELECT 0xD0
#define KB_KPENTER 0xDA
#define KB_ACPIWAKE 0xDE
#define KB_END 0xE9
#define KB_CURLEFT 0xEB
#define KB_HOME 0xEC
#define KB_INSERT 0xF0
#define KB_DELETE 0xF1
#define KB_CURDOWN 0xF2
#define KB_CURRIGHT 0xF4
#define KB_CURUP 0xF5
#define KB_PAGEDOWN 0xFA
#define KB_PAGEUP 0xFD

/* Extra keys */
#define KB_EXTRA1 0x61

// And so on and so forth
// https://wiki.osdev.org/PS/2_Keyboard

#endif /* API_KEYSCAN_H_ */






































