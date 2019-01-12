#ifndef INTERRUPT_HANDLER_FACTORY_HPP_
#define INTERRUPT_HANDLER_FACTORY_HPP_

#include <cstddef>

class HandlerFactory
{
public:
	void load(char* start, char* end);
	void build(char* destination, int irq);
	size_t length();
private:
	char* m_start;
	char* m_end;
	size_t m_irq_offset;
};

#endif
