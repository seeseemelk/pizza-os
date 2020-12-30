#ifndef INTERRUPT_HANDLER_FACTORY_HPP_
#define INTERRUPT_HANDLER_FACTORY_HPP_

#include "libc/stddef.hpp"

class HandlerFactory
{
public:
	void load(char* start, char* end);
	void build(char* destination, int irq);
	size_t length();
private:
	char* m_start;
	size_t m_length;
	size_t m_irq_offset;
};

#endif
