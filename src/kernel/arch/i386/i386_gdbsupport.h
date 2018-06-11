/*
 * i386_gdbsupport.h
 *
 *  Created on: May 27, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_GDBSUPPORT_H_
#define ARCH_I386_I386_GDBSUPPORT_H_

void putDebugChar(char c);	/* write a single character      */
int getDebugChar();	/* read and return a single char */
void exceptionHandler();	/* assign an exception handler   */

#endif /* ARCH_I386_I386_GDBSUPPORT_H_ */
