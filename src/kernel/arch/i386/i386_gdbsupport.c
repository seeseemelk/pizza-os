#include "arch/i386/i386_gdbsupport.h"
#include "arch/i386/i386_interrupt.h"
#include "dev/eserial.h"

void putDebugChar(char c)
{
	eserial_putchar(c);
}

int getDebugChar()
{
	return (int) eserial_readchar();
}

void exceptionHandler(int exceptionNumber, void* target)
{
	interrupt_debug_jmp(exceptionNumber, target);
}
