/*
 * io.hpp
 *
 *  Created on: Dec 22, 2018
 *      Author: seeseemelk
 */

#ifndef IO_HPP_
#define IO_HPP_

#include <cstdint>

namespace IO
{
	void outb(u16 port, u8 value);
	void outw(u16 port, u16 value);
	void outl(u16 port, u32 value);

	u8 inb(u16 port);
	u16 inw(u16 port);
	u32 inl(u16 port);

	class Port
	{
	public:
		Port(u16 port);

		void outb(u8 value) const;
		void outw(u16 value) const;
		void outl(u32 value) const;
		u8 inb() const;
		u16 inw() const;
		u32 inl() const;

		void outb(int value) const;
		void outw(int value) const;
	private:
		u16 m_port;
	};
}

#endif /* IO_HPP_ */