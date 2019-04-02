#include "interrupt/handler.hpp"
#include "interrupt.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include <cstdint>

using namespace Interrupt;

struct PageFaultCode
{
	u32 present : 1;
	u32 write : 1;
	u32 user : 1;
	u32 rsvd : 1;
	u32 fetch : 1;
	u32 pk : 1;
	u32 sgx : 1;
} __attribute__((packed));

void handle_page_fault(int error_code)
{
	u32 address = CPU::get_cr2();
	log("Page Fault at address 0x%X, error: 0x%X", address, error_code);

	//PageFaultCode code = static_cast<PageFaultCode>(error_code);
	PageFaultCode& code = *reinterpret_cast<PageFaultCode*>(&error_code);
	if (code.present)
		log("  - Caused by a page-level protection violation.");
	else
		log("  - Caused by a non-present page.");

	if (code.write)
		log("  - Caused by a write.");
	else
		log("  - Caused by a read.");

	if (code.fetch)
		log("  - Caused by an instruction fetch.");
	else
		log("  - Caused by an instruction that performed a read or write.");

	if (code.user)
		log("  - Caused in user-mode.");
	else
		log("  - Caused in supervisor-mode.");

	if (code.rsvd)
		log("  - Caused by a reserved bit violation (some bit is set to 1 in some paging-structure entity).");

	if (code.pk)
		log("  - Caused by a protection key violation.");

	if (code.sgx)
		log("  - Caused by a violation of SGX-specific access-control requirements.");

	CPU::hang();
}

extern "C" void handle_interrupt(int irq, int error_code)
{
	//log("IRQ: 0x%X, error: 0x%X", irq, error_code);
	//log("Start handling IRQ 0x%X", irq);
	switch (irq)
	{
		case 0x00:
			log("Divide Error");
			CPU::hang();
			break;
		case 0x01:
			log("Debug Exception");
			CPU::hang();
			break;
		case 0x02:
			log("NPI Interrupt");
			CPU::hang();
			break;
		case 0x03:
			log("Break Point");
			CPU::hang();
			break;
		case 0x04:
			log("Overflow");
			CPU::hang();
			break;
		case 0x05:
			log("BOUND Range Exceeded");
			CPU::hang();
			break;
		case 0x06:
			log("Invalid Opcode");
			CPU::hang();
			break;
		case 0x07:
			log("Device Not Available (No Math Coprocessor)");
			CPU::hang();
			break;
		case 0x08:
			log("Double Fault, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x09:
			log("Coprocessor Segment Overrun");
			CPU::hang();
			break;
		case 0x0A:
			log("Invalid TSS, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x0B:
			log("Segment Not Present, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x0C:
			log("Stack-Segment Fault, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x0D:
			log("General Protection, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x0E:
			handle_page_fault(error_code);
			break;
		case 0x0F:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1F:
			log("Reserved IRQ 0x0F, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x10:
			log("Math Fault");
			CPU::hang();
			break;
		case 0x11:
			log("Alignment Check, error: 0x%X", error_code);
			CPU::hang();
			break;
		case 0x12:
			log("Machine Check");
			CPU::hang();
			break;
		case 0x13:
			log("SIMD Floating-Point Exception");
			CPU::hang();
			break;
		case 0x14:
			log("Virtualization Exception");
			CPU::hang();
			break;
		default:
			if ((irq == 7 || irq == 15) && !PIC::is_served(static_cast<u8>(irq)))
			{
				static int spur_count = 0;
				log("Spurius IRQ 0x%X (spur_count = %d)", irq, ++spur_count);
				return;
			}
			log("Received external IRQ 0x%X", irq);
			PIC::send_eoi(static_cast<u8>(irq));
			break;
	}
}











