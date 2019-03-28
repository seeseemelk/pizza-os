#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include "result.hpp"
#include "slab.hpp"
#include "result.hpp"
#include "paging.hpp"
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
		int m_pid;
		int m_uid;
		int m_gid;
		ProcessState m_state;

		Result<Process*> fork();
		bool is_current();
		void switch_to();
		void kill();
		ResultState map_page(void* address);
		ResultState map_pages(void* address, size_t blocks);

	private:
		u32 m_page_directory;
		intptr_t m_entry_point;

		void load_pagetable();
	};

	extern Slab<Process> allocator;
	extern Process* current_process;

	void init();
	Result<Process*> exec_initrd(const char* filename);
}

#endif
