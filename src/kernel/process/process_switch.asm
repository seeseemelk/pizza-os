[bits 32]

section .text:
	global asm_enter_process_interrupt
	asm_enter_process_interrupt:
		push ebp
		mov [esp_kernel], esp
		mov eax, [esp + 8]
		mov esp, eax

		mov ax, 0x23
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax

		iret

	global asm_enter_process_normal
	asm_enter_process_normal:
		push ebp
		mov [esp_kernel], esp
		mov eax, [esp + 8]
		mov esp, eax
		pop ebp
		ret

	global asm_enter_kernel
	asm_enter_kernel:
		push ebp
		mov eax, [esp + 8]
		mov [eax], esp

		mov eax, [esp_kernel]
		mov esp, eax

		pop ebp
		ret

section .rodata:
	esp_kernel dq 0
