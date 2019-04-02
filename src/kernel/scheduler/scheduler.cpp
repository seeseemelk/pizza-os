#include "scheduler.hpp"
#include "interrupt.hpp"
#include "process.hpp"
#include "debug.hpp"
#include "cpu.hpp"

using namespace Scheduler;

void Scheduler::run()
{
	while (1)
	{
		Proc::current_process->enter_process();
		if (Proc::current_process->validate_stack_protector() == ResultState::FAIL)
		{
			log("Broken stack protector");
			CPU::hang();
		}

		Interrupt::handle_interrupt();
	}
}
