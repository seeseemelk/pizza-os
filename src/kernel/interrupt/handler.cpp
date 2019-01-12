#include "handler.hpp"
#include <cstdint>

void HandlerFactory::load(char* start, char* end)
{
	m_start = start;
	m_end = end;
	// Find the location of the magic byte 0xFFFFFFFF
	u32* magic_ptr = reinterpret_cast<u32*>(m_start);
	while (*magic_ptr != 0xFFFFFFFF)
		magic_ptr = reinterpret_cast<u32*>(reinterpret_cast<u32>(magic_ptr) + 1);
	m_irq_offset = reinterpret_cast<size_t>(magic_ptr) - reinterpret_cast<size_t>(m_start);
}

void HandlerFactory::build(char* destination, int irq)
{
	// Copy code
	char* ptr = m_start;
	while (ptr < m_end)
		*(destination++) = *(ptr++);

	// Store IRQ value
	u32* irq_ptr = reinterpret_cast<u32*>(destination + m_irq_offset);
	*irq_ptr = irq;
}

size_t HandlerFactory::length()
{
	return m_end - m_start;
}
