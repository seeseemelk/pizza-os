#ifdef TESTING
#include "test.hpp"
#include "process.hpp"
#include "scheduler.hpp"

extern "C" u8 test_syscall_payload_start;
extern "C" u8 test_syscall_payload_end;
static const size_t size = &test_syscall_payload_end - &test_syscall_payload_start;

static void writeSyscallPayload(u8* mem)
{
	log("Writing syscall payload with size %d", size);
	log("Copying from 0x%X to 0x%X", &test_syscall_payload_start, mem);
	for (size_t i = 0; i < size; i++)
	{
		mem[i] = (&test_syscall_payload_start)[i];
	}
}

static void setSyscallParameters(u8* mem, u32 syscall, u32 ebx, u32 ecx, u32 edx)
{
	log("Setting syscall parameters");
	for (size_t i = 0; i < size; i++)
	{
		u32* mem32 = reinterpret_cast<u32*>(mem + i);
		switch (*mem32)
		{
		case 0x00112233:
			*mem32 = syscall;
			break;
		case 0x44556677:
			*mem32 = ebx;
			break;
		case 0x8899aabb:
			*mem32 = ecx;
			break;
		case 0xccddeeff:
			*mem32 = edx;
			break;
		}
	}
}

u32 Test::Utils::testSyscall(u32 syscall, u32 ebx, u32 ecx, u32 edx)
{
	using namespace Test::Asserts;

	if (Proc::current_process == nullptr)
	{
		log("Loading new process");
		require(Proc::exec_empty());
		//require(Proc::current_process->map_page(reinterpret_cast<void*>(KB(4))));
	}

	u8* mem = reinterpret_cast<u8*>(KB(4));

	if (!Proc::current_process->is_mapped(mem))
		require(Proc::current_process->map_page(reinterpret_cast<void*>(KB(4))));

	writeSyscallPayload(mem);
	setSyscallParameters(mem, syscall, ebx, ecx, edx);

	*reinterpret_cast<u32*>(0x1404) = 0x13371337;

	log("Executing syscall");
	while (*reinterpret_cast<u32*>(0x1404) != 0xDEADDEAD)
	{
		log("Value is: 0x%X", *reinterpret_cast<u32*>(0x1404));
		Scheduler::run_process();
	}
	log("Signal detected, process payload finished");

	return *reinterpret_cast<u32*>(0x1400);
}
#endif
