[bits 32]

section .text
	global syscall
	syscall:
		push ebp
		mov ebp, esp
		push ebx

		mov edx, [ebp + 0x14]
		mov ecx, [ebp + 0x10]
		mov ebx, [ebp + 0x0C]
		mov eax, [ebp + 0x08]
		int 80h

		pop ebx
		leave
		ret
