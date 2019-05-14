#include "test.hpp"
#include "process.hpp"

u32 Test::Utils::testSyscall(u32 syscall, u32 ebx, u32 ecx, u32 edx)
{
	if (Proc::current_process == nullptr)
		Test::Asserts::require(Proc::exec_empty());

	Proc::current_process->map_page(reinterpret_cast<void*>(KB(4)));

	/* Shell code to use for testing:
	 * 0:  b8 33 22 11 00          mov    eax,0x112233
	 * 5:  bb 77 66 55 44          mov    ebx,0x44556677
	 * a:  b9 bb aa 99 88          mov    ecx,0x8899aabb
	 * f:  ba ff ee dd cc          mov    edx,0xccddeeff
	 * 14: cd 50                   int    0x50
	 * 16: a3 04 10 00 00          mov    ds:0x1004,eax
	 * 1b: 31 c0                   xor    eax,eax
	 * 1d: cd 80                   int    0x80
	 */

	u8* mem = reinterpret_cast<u8*>(KB(4));

	mem[0x00] = 0xB8;

	mem[0x05] = 0xBB;

	mem[0x0A] = 0xB9;

	mem[0x0F] = 0xBA;

	mem[0x14] = 0xCD;
	mem[0x15] = 0x80;

	mem[0x16] = 0xA3;
	mem[0x17] = 0x04;
	mem[0x18] = 0x10;
	mem[0x19] = 0x00;
	mem[0x1A] = 0x00;

	mem[0x1B] = 0x31;
	mem[0x1C] = 0xC0;

	mem[0x1D] = 0xCD;
	mem[0x1E] = 0x80;

	*reinterpret_cast<u32*>(mem + 0x01) = syscall;
	*reinterpret_cast<u32*>(mem + 0x06) = ebx;
	*reinterpret_cast<u32*>(mem + 0x0B) = ecx;
	*reinterpret_cast<u32*>(mem + 0x10) = edx;

	Proc::current_process->enter_process();

	return *reinterpret_cast<u32*>(mem);
}
