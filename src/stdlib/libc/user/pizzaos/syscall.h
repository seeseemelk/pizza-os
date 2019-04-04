#ifndef PIZZAOS_SYSCALL_H_
#define PIZZAOS_SYSCALL_H_

#include <pizzaos/stdint.h>

/* Raw syscall. */
extern u32 syscall(u32 eax, ...);

/* Process management syscalls. */
void syscall_exit(void);

/* Ramdisk syscalls. */
u32 syscall_ramdisk_open(const char* filename);
void syscall_ramdisk_close(u32 descriptor);
void syscall_ramdisk_seek(u32 descriptor, u32 offset, u32 location);
u32 syscall_ramdisk_read(u32 descriptor, char* buf, u32 max_bytes);
void syscall_ram_disable();

#endif
