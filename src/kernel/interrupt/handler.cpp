#include "handler.hpp"
#include "debug.hpp"
#include <cstdint>
#include <cstring>

void HandlerFactory::load(char* start, char* end)
{
	m_start = start;
	m_length = end - start;
	// Find the location of the magic byte 0xFFFFFFFF
	u32* magic_ptr = reinterpret_cast<u32*>(m_start);
	while (*magic_ptr != 0xFFFFFFFF)
		magic_ptr = reinterpret_cast<u32*>(reinterpret_cast<u32>(magic_ptr) + 1);
	m_irq_offset = reinterpret_cast<size_t>(magic_ptr) - reinterpret_cast<size_t>(m_start);
}

void HandlerFactory::build(char* destination, int irq)
{
	// Get pointer to IRQ location.
	u32* irq_ptr = reinterpret_cast<u32*>(destination + m_irq_offset);

	// Copy code.
	memcpy(destination, m_start, m_length);

	// Store IRQ value.
	*irq_ptr = irq;
}

size_t HandlerFactory::length()
{
	return m_length;
}
