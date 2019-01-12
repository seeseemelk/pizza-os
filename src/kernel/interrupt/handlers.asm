[bits 32]

section .text

extern handle_interrupt

; Actual interrupt handler code
%macro inth_no_errorcode 1
	pushad
	push strict dword %1
	push 0
	call handle_interrupt
	add eax, 8
	popad
	iretd
%endmacro

%macro inth_errorcode 1
	pushad
	push strict dword %1
	mov eax, [esp+4]
	push eax
	call handle_interrupt
	add eax, 8
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

;; Macros to repeat the interrupt handlers
;%macro repeat_inth_no_errorcode 1
;	%rep %1
;	global IHANDLE_%+i
;	IHANDLE_%+i:
;	inth_no_errorcode 1
;	%assign i i+1
;	%endrep
;%endmacro
;
;%macro repeat_inth_errorcode 1
;	%rep %1
;	inth_no_errorcode 1
;	%assign i i+1
;	%endrep
;%endmacro
;
;; Write the interrupt handlers
;%assign i 0
;; Exception handlers
;repeat_inth_no_errorcode 8
;repeat_inth_errorcode 1
;repeat_inth_no_errorcode 1
;repeat_inth_errorcode 5
;repeat_inth_no_errorcode 1
;repeat_inth_errorcode 1
;repeat_inth_no_errorcode 3
;
;; Device driver handlers
;%assign i 0x20
;repeat_inth_errorcode 0x10
;
