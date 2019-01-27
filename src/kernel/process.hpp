#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include "result.hpp"
#include "slab.hpp"

namespace Processes
{
	enum ProcessState
	{
		STARTING,
		ALIVE,
		WAITING,
		DEAD
	};

	class Process
	{
	public:
		int pid;
		int uid;
		int gid;
		ProcessState state;

	private:
	};

	extern Slab<Process> allocator;
	extern Process* current_process;

	void init();
	Result<Process*> exec_initrd(const char* filename);
	Result<Process*> fork();
	void switch_to(Process& process);
	void kill(Process& process);
}

#endif
