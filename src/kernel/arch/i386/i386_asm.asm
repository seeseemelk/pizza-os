
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

global thread_save
thread_save:
	enter 0, 0

	;Get a pointer to the data structure
	mov edx, [ebp+4]

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
	mov dword [edx+6], .exit

	;Store EFLAGS
	pushf
	pop eax
	mov [edx+10], eax

	;Store ESP
	mov [edx+14], esp

	;Store EBP
	mov [edx+16], ebp

	;Store ESI
	mov [edx+22], esi

	;Store EDI
	mov [edx+26], edi

	;Return with a value of 0
	.exit:
	leave
	mov eax, 0
	ret

global thread_enter
thread_enter:
	cli
	.loop:
	hlt
	jmp .loop
	ret





