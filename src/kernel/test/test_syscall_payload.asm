[bits 32]

section .text

global test_syscall_payload_start
global test_syscall_payload_end

test_syscall_payload_start:
	.start:
	; Setup stack
	mov	eax, 0x2000 - 4
	mov ebp, eax
	mov esp, eax

	; Load values
	mov eax, 0x00112233
	mov ebx, 0x44556677
	mov ecx, 0x8899aabb
	mov edx, 0xccddeeff

	; Execute syscall
	int 0x80
	mov [0x1400], eax

	; Signal that the process finished executing and is now dead
	mov dword [0x1404], 0xDEADDEAD

	; Yield
	.yield:
		mov eax, 0x5
		int 0x80

		mov eax, [0x1404]
		cmp eax, 0x13371337
		je .start
	jmp .yield

	; Exit process
	;xor eax, eax
	;int 0x80
test_syscall_payload_end:
