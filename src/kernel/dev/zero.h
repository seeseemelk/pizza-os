/**
 * Represents a zero device that lives in /dev/zero.
 * A read from this will always fill the buffer with zeroes,
 * a write from this will ignore the write, but also pretend
 * that the write was successful.
 */

#ifndef DEV_ZERO_H_
#define DEV_ZERO_H_

void zero_init();

#endif /* DEV_ZERO_H_ */
