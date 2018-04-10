/*
 * asmfn.h
 * All functions described in this header file are implemented in assembler.
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_ASM_H_
#define ARCH_I386_I386_ASM_H_

#include <stddef.h>
#include "thread.h"

extern void asm_enable_cr0_pepg();
extern void asm_load_cr3_page_dir(size_t address);
extern void asm_invlpg(volatile void* virt);
extern void cli();
extern void sti();
extern void asm_reload_segment_registers();
extern void thread_save(thread_data* data);
extern int thread_enter(thread_data* data);

#endif /* ARCH_I386_I386_ASM_H_ */
