#include "io.hpp"
#include <cstdint>

using namespace IO;

Port::Port(u16 port)
	: m_port(port)
{
}

void Port::outb(u8 value) const
{
	asm ("outb %1, %0" :: "d" (m_port), "a" (value));
}

void Port::outb(int value) const
{
	outb(static_cast<u8>(value));
}

void Port::outw(u16 value) const
{
	asm ("outw %1, %0" :: "d" (m_port), "a" (value));
}

void Port::outw(int value) const
{
	outw(static_cast<u16>(value));
}

void Port::outl(u32 value) const
{
	asm ("outl %1, %0" :: "d" (m_port), "a" (value));
}

u8 Port::inb() const
{
	u8 value;
	asm volatile ("inb %1, %0" : "=a" (value) : "d" (m_port));
	return value;
}

u16 Port::inw() const
{
	u16 value;
	asm volatile ("inw %1, %0" : "=a" (value) : "d" (m_port));
	return value;
}

u32 Port::inl() const
{
	u32 value;
	asm volatile ("inl %1, %0" : "=a" (value) : "d" (m_port));
	return value;
}


