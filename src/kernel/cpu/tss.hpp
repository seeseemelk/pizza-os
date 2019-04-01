#include "cpu.hpp"
#include <cstdint>

namespace CPU
{

struct TSS
{
	u16 previous_task_link;
	u16 _1;

	u32 esp0;

	u16 ss0;
	u16 _2;

	u32 esp1;

	u16 ss1;
	u16 _3;

	u32 esp2;

	u16 ss2;
	u16 _4;

	// Fixed here
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;

	u16 es;
	u16 _5;

	u16 cs;
	u16 _6;

	u16 ss;
	u16 _7;

	u16 ds;
	u16 _8;

	u16 fs;
	u16 _9;

	u16 gs;
	u16 _10;

	u16 ldt_segment_selector;
	u16 _11;

	u16 _12;
	u16 iomap_base;
};

}
