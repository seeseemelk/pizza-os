/*
 * pic.h
 *
 *  Created on: Jan 25, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_PIC_H_
#define ARCH_I386_I386_PIC_H_

#include "cdefs.h"

void pic_send_eoi(u8 irq);
void pic_init();

#endif /* ARCH_I386_I386_PIC_H_ */
