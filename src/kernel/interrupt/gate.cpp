#include "interrupt.hpp"

using namespace Interrupt;

void Gate::set_offset(size_t address)
{
	offset_1 = static_cast<u16>(address);
	offset_2 = static_cast<u16>(address >> 16);
}

size_t Gate::get_offset()
{
	return static_cast<size_t>(offset_1) |
			(static_cast<size_t>(offset_2) << 16);
}

void Gate::set_type(GateType type)
{
	zero = 0;
	switch (type)
	{
	case GateType::TASK:
		type_2 = 0b101;
		break;
	case GateType::INTERRUPT:
		type_1 = 0b000;
		type_2 = 0b110;
		break;
	case GateType::TRAP:
		type_1 = 0b000;
		type_2 = 0b111;
		break;
	}
}
