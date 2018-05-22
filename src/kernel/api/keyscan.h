/**
 * Contains keyboard scancode definitions
 */

#ifndef API_KEYSCAN_H_
#define API_KEYSCAN_H_

typedef int SCANCODE;

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

#define SC_F9 0x01
#define SC_F5 0x03
#define SC_F3 0x04
#define SC_F1 0x05
#define SC_F2 0x06
#define SC_F12 0x07
#define SC_F10 0x09
#define SC_F8 0x0A
#define SC_F6 0x0B
#define SC_F4 0x0C
#define SC_TAB 0x0D
#define SC_BACKTICK 0x0E
#define SC_LALT 0x11
#define SC_LSHIFT 0x12
#define SC_LCONTROL 0x14
#define SC_Q 0x15
#define SC_1 0x16
#define SC_Z 0x1A
#define SC_S 0x1B
// And so on and so forth
// https://wiki.osdev.org/PS/2_Keyboard

#endif /* API_KEYSCAN_H_ */
