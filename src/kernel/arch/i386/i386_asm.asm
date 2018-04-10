
global asm_enable_cr0_pepg
asm_enable_cr0_pepg:
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	ret

global asm_load_cr3_page_dir
asm_load_cr3_page_dir:
	mov eax, [esp+4]
	mov cr3, eax
	ret

global asm_invlpg
asm_invlpg:
	mov eax, [esp+4]
	invlpg [eax]
	ret

global cli
cli:
	cli
	ret

global sti
sti:
	sti
	ret

global asm_reload_segment_registers
asm_reload_segment_registers:
	jmp dword 8:.jump
.jump:
	mov ax, 16
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov ss, ax
	ret

; These twe functions may require some explanation;
; thread_save does not store the call stack, but rather the call stack depth.
; This means that if a function calls thread_save and then thread_enter, the
; .second_exit segment will return as if it part of thread_enter.
; This also means that you shouldn't have any other function calls between
; thread_save and thread_enter.
; thread_enter will return 0 normally, but if the exit routine of thread_save is executed
; it will return 1.
global thread_save
thread_save:
	enter 0, 0

	;Get a pointer to the data structure
	mov edx, [ebp+8]

	;Store DS
	mov ax, ds
	mov [edx], ax

	;Store SS
	mov ax, ss
	mov [edx+2], ax

	;Store CS
	mov ax, cs
	mov [edx+4], ax

	;Store EIP
	mov dword [edx+6], .second_exit

	;Store EFLAGS
	pushfd
	pop eax
	mov [edx+10], eax

	;Store ESP
	mov [edx+14], esp

	;Store EBP
	mov [edx+18], ebp

	;Store ESI
	mov [edx+22], esi

	;Store EDI
	mov [edx+26], edi

	;Return with no value (Returns as thread_save)
	.exit:
	leave
	ret

	;Return with a value of 1 (Returns as thread_enter)
	.second_exit:
	leave
	mov eax, 1
	ret

extern kernel_panic
global thread_enter
thread_enter:
	enter 0, 0
	mov edx, [ebp+8]

	;Restore DS and SS
	mov ax, [edx+0]
	mov ds, ax
	mov ax, [edx+2]
	mov ss, ax

	;Restore other registers
	mov esp, [edx+14]
	mov ebp, [edx+18]
	mov esi, [edx+22]
	mov edi, [edx+26]

	;Perform return
	push dword [edx+6]
	sti
	ret

	leave
	mov eax, 0
	ret

msg db "Hello world", 0



