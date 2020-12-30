#include "scheduler.hpp"

#include "cpu.hpp"
#include "debug.hpp"
#include "interrupt.hpp"
#include "process.hpp"

using namespace Scheduler;

void Scheduler::run_process()
{
	log("Entering process");
	Proc::current_process->enter_process();
	if (Proc::current_process->validate_stack_protector() == ResultState::FAIL)
	{
		log("Broken stack protector");
		CPU::hang();
	}

	log("Handling interrupts");
	Interrupt::handle_interrupt();
	log("Finished");
}

void Scheduler::run()
{
	Proc::Process* mcp = Proc::current_process;
	while (mcp->m_state != Proc::DEAD)
	{
		run_process();
		Proc::current_process->m_next_process->switch_to();
	}

	mcp->close_handle();
	log("Main process died");
}
