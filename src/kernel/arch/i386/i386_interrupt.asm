[cpu 386]
[bits 32]
[global inth_start]
[global inth_end]

extern cpu_int_handler

my_loop:
	jmp $

; ISR template
; Note that if this is changed you might need to also change the line
;     *((u32*)(handler+N)) = i;
; in the function init_handlers() in i386_interrupt.c
section .data
inth_start:
	pushad
	push strict dword 0
	mov eax, cpu_int_handler
	call eax
	pop eax
	popad
	iretd
inth_end:
