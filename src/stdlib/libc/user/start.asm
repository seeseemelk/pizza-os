[bits 32]

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section _pretext
global _start:function (_start.end - _start)
_start:
	jmp $
	mov ebp, stack_top
	mov esp, ebp

	call _init
	call main
	call _fini

.hang:
	jmp .hang
.end:

extern _init
extern _fini
extern main
