/*
 * asmfn.h
 * All functions described in this header file are implemented in assembler.
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_ASMFN_H_
#define ARCH_I386_ASMFN_H_

#include <stddef.h>

extern void asm_enable_cr0_pepg();
extern void asm_load_cr3_page_dir(size_t address);
extern void asm_invlpg(volatile void* virt);
extern void cli();
extern void sti();
extern void asm_reload_segment_registers();

#endif /* ARCH_I386_ASMFN_H_ */
