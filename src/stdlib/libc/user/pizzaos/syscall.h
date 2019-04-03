#ifndef PIZZAOS_SYSCALL_H_
#define PIZZAOS_SYSCALL_H_

#include <pizzaos/stdint.h>

extern u32 syscall(u32 eax, ...);
void syscall_exit(void);

#endif
