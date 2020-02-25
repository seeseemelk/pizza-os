#ifndef PIZZAOS_SYSCALLS_NUMS_H_
#define PIZZAOS_SYSCALLS_NUMS_H_

// Common Syscalls
#define SYSCALL_EXIT 0

// VFS Syscalls
#define SYSCALL_VFS_ISOPEN 1

// Process Syscalls
#define SYSCALL_PROCESS_PID 2
#define SYSCALL_PROCESS_GID 3
#define SYSCALL_PROCESS_UID 4
#define SYSCALL_PROCESS_YIELD 5

// Debug Syscalls
#define SYSCALL_DEBUG_PRINT 255

#endif
