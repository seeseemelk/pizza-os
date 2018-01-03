
global asm_enable_cr0_pepg
global asm_load_cr3_page_dir

asm_enable_cr0_pepg:
	push eax
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	pop eax
	ret

asm_load_cr3_page_dir:
	pop eax
	mov cr3, eax
	ret
