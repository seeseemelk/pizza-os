#include "scheduler.hpp"
#include "interrupt.hpp"
#include "process.hpp"
#include "debug.hpp"
#include "cpu.hpp"

using namespace Scheduler;

void Scheduler::run()
{
	Proc::Process* mcp = Proc::current_process;
	while (mcp->m_state != Proc::DEAD)
	{
		Proc::current_process->enter_process();
		if (Proc::current_process->validate_stack_protector() == ResultState::FAIL)
		{
			log("Broken stack protector");
			CPU::hang();
		}

		Interrupt::handle_interrupt();

		Proc::current_process->m_next_process->switch_to();
	}

	mcp->close_handle();
	log("Main process died");
}
