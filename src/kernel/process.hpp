#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include "result.hpp"
#include "slab.hpp"
#include "result.hpp"
#include <cstdint>

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

		Result<Process*> fork();
		void switch_to();
		void kill();
		ResultState map_page(void* address);
		ResultState map_pages(void* address, size_t blocks);

	private:
	};

	extern Slab<Process> allocator;
	extern Process* current_process;

	void init();
	Result<Process*> exec_initrd(const char* filename);
}

#endif
