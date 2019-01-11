/*
 * interrupts.hpp
 *
 *  Created on: Jan 9, 2019
 *      Author: seeseemelk
 */

#ifndef INTERRUPT_HPP_
#define INTERRUPT_HPP_

#include <cstdint>
#include <cstddef>

/* INTERRUPT LIST
 * 00	#DE	Divide Error
 * 01	#DB	Debug Exception
 * 02		NMI Interrupt
 * 03	#BP	Break Point
 * 04	#OF	Overflow
 * 05	#BR	BOUND Range Exceeded
 * 06	#UD	Invalid Opcode
 * 07	#NM	Device Not Available (No Math Coprocessor)
 * 08	#DF	Double Fault (ERROR CODE)
 * 09		Coprocessor Segment Overrun (reserved)
 * 0A	#TS	Invalid TSS (ERROR CODE)
 * 0B	#NP	Segment not present (ERROR CODE)
 * 0C	#SS	Stack-Segment Fault (ERROR CODE)
 * 0D	#GP	General Protection (ERROR CODE)
 * 0E	#PF	Page Fault (ERROR CODE)
 * 0F		(reserved)
 * 10	#MF	Math Fault
 * 11	#AC	Alignment Check (ERROR CODE)
 * 12	#MC	Machine Check
 * 13	#XM	SIMD Floating-Point Exception
 * 14	#VE	Virtualization Exception
 * 15-1F	(reserved)
 */

/*
 * Task Gate:
 * Interrupt Gate:
 * Trap Gate:
 */

namespace Interrupt
{

enum GateType
{
	TASK,
	INTERRUPT,
	TRAP
};

struct Gate
{
	// Byte 1
	u16 offset_1;
	// Byte 2
	u16 segment;

	// Byte 3
	u16 _1 : 5;
	u16 type_1 : 3;
	u16 type_2 : 3;
	u16 size : 1;
	u16 zero : 1;
	u16 dpl : 2;
	u16 present : 1;

	// Byte 4
	u16 offset_2;

	// Members
	void set_offset(size_t address);
	size_t get_offset();
	void set_type(GateType type);
} __attribute__((packed));

struct IDT
{
	Gate gates[256];
} __attribute__((aligned(8),packed));

struct IDTR
{
	u16 limit;
	u32 address;
} __attribute__((packed));

extern IDT idt;
extern IDTR idtr;
Gate& get_handler(int irq);

void init();
void disable();
void enable();

}


#endif /* INTERRUPT_HPP_ */
