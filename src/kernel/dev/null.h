/**
 * Represents a null device that lives in /dev/null.
 * A read or write to and from this always returns 0 bytes read/written.
 */

#ifndef DEV_NULL_H_
#define DEV_NULL_H_

void null_init();

#endif /* DEV_NULL_H_ */
