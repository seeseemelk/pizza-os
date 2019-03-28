; This multiboot compliant header was taken from
; http://wiki.osdev.org/Bare_Bones_with_NASM
[bits 32]
MBALIGN equ 1<<0
MEMINFO equ 1<<1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
	mov ecx, ebx

	; Initialise the system page directory and load it.
	mov dword [_system_page_directory - 0xC0000000], 0x00000003
	mov dword [_system_page_directory - 0xC0000000 + 3072], 0x00000003
	mov eax, _system_default_page_table
	sub eax, 0xC0000000
	and eax, 0xFFFFF000
	or [_system_page_directory - 0xC0000000], eax
	or [_system_page_directory - 0xC0000000 + 3072], eax

	mov eax, 0
	mov ebx, 0
	.loop:
		mov dword [_system_default_page_table - 0xC0000000 + eax], 3
		or [_system_default_page_table - 0xC0000000 + eax], ebx
		add eax, 4
		add ebx, 0x1000
		cmp eax, 4096
		je .loop_end
		jmp .loop
	.loop_end:

	mov eax, _system_page_directory - 0xC0000000
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	; Initialise stack and store boot parameters
	mov ebp, stack_top
	mov esp, stack_top
	push esp
	push ebx
	add ecx, 0xC0000000
	push ecx

	; Boot kernel_main
	extern kernel_main
	call kernel_main + 0xC0000000
	cli
.hang:
	hlt
	jmp .hang
.end:

section .bss
align 4096
global _system_page_directory
_system_page_directory resd 1024

align 4096
global _system_default_page_table
_system_default_page_table resd 1024
