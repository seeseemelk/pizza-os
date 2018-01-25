[cpu 386]
[global inth_start]
[global inth_end]

extern cpu_int_handler

section .data
inth_start:
	;push dword 0
	;call cpu_int_handler
	;add eax, 4
	;iret
	jmp $
inth_end:
