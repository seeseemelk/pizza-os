
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
