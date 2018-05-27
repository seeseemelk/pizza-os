/**
 * Eserial stands for early-serial.
 * As it is meant to be available for the very earliest moment the kernel boots,
 * it does not interact with any APIs, busses, or any other manager of any kind.
 */

#ifndef DEV_ESERIAL_H_
#define DEV_ESERIAL_H_

void eserial_init();
void eserial_putchar(char c);
char eserial_readchar();

#endif /* DEV_ESERIAL_H_ */
