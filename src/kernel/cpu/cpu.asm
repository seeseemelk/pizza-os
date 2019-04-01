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

global asm_load_cr3
asm_load_cr3:
	mov eax, [esp + 4]
	mov cr3, eax
	ret

global asm_enter_ring_3
asm_enter_ring_3:
	mov ebx, [esp + 4]

	push 0x23 ; User data segment (0x20) with bottom 2 bits for ring 3
	push 0
	pushf
	push 0x1B ; User code segment (0x18) with bottom 2 bits for ring 3
	push ebx

	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	iret
