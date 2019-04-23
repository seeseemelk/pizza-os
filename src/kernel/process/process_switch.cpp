#include "process.hpp"
#include "debug.hpp"

using namespace Proc;

bool Process::is_current()
{
	return this == Proc::current_process;
}

extern "C" void asm_switch_to_process();
void Process::switch_to()
{
	if (!is_current())
	{
		log("Reloading pagetable");
		CPU::set_ring3_syscall_stack(m_syscall_stack + 128);
		load_pagetable();
	}
}

extern "C" void asm_enter_process_interrupt(void*);
extern "C" void asm_enter_process_normal(void*);
void Process::enter_process()
{
	switch_to();
	if (m_state == ProcessState::ALIVE)
	{
		asm_enter_process_normal(m_esp);
	}
	else
	{
		m_state = ProcessState::ALIVE;
		asm_enter_process_interrupt(m_esp);
	}
}

extern "C" void asm_enter_kernel(void**);
void Process::enter_kernel()
{
	asm_enter_kernel(&m_esp);
}

void Process::load_pagetable()
{
	Paging::load_directory(m_page_directory);
}
