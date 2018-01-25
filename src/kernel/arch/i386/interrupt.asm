[cpu 386]
[bits 32]
[global inth_start]
[global inth_end]

extern cpu_int_handler

my_loop:
	jmp $

section .data
inth_start:
	push strict dword 0
	mov eax, cpu_int_handler
	call eax
	add esp, 4
	iretd
inth_end:
