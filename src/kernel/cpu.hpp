#ifndef CPU_HPP_
#define CPU_HPP_

namespace CPU
{
	void init();
	void init_gdt();
	void halt();
	void hang();
}

#endif
