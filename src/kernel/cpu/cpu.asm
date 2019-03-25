[bits 32]

global load_kernel_segment_registers
load_kernel_segment_registers:
	; Set segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov ss, ax
	jmp 0x08:.jump
	.jump:
	ret

global asm_get_cr2
asm_get_cr2:
	mov eax, cr2
	ret
