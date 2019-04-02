[bits 32]

section .text

extern handle_interrupt

; Actual interrupt handler code
%macro inth_no_errorcode 1
	pushad
	push 0
	push strict dword %1
	mov eax, handle_interrupt
	call eax
	add esp, 8
	popad
	iretd
%endmacro

%macro inth_errorcode 1
	pushad
	mov eax, [esp+32]
	push eax
	push strict dword %1
	mov eax, handle_interrupt
	call eax
	add esp, 8
	popad
	iretd
%endmacro

global INTH
global INTH_END
INTH:
inth_no_errorcode 0xFFFFFFFF
INTH_END:

global INTH_ERR
global INTH_ERR_END
INTH_ERR:
inth_errorcode 0xFFFFFFFF
INTH_ERR_END:
