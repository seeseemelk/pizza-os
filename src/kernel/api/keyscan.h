/**
 * Contains keyboard scancode definitions
 */

#ifndef API_KEYSCAN_H_
#define API_KEYSCAN_H_

typedef char SCANTYPE;
typedef int SCANCODE;

typedef struct scancode_t scancode_t;
struct scancode_t
{
	SCANCODE code;
	SCANTYPE type;
};

#endif /* API_KEYSCAN_H_ */
