#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include "result.hpp"
#include "slab.hpp"
#include "result.hpp"
#include "paging.hpp"
#include "elf.hpp"
#include <cstdint>

namespace Proc
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
		u32 m_entry_point;

		Result<Process*> fork();
		ResultState exec_elf(Elf& elf);
		bool is_current();
		void switch_to();
		void kill();

		void enter_process();
		void enter_kernel();

		ResultState map_page(void* address);
		ResultState map_pages(void* address, size_t blocks);

		ResultState validate_stack_protector();

	private:
		u32 m_page_directory;
		u32 m_syscall_stack[34];
		void* m_esp;

		void load_pagetable();
		void init_stack();
	};

	extern Slab<Process> allocator;
	extern Process* current_process;

	void init();
	Result<Process*> exec_initrd(const char* filename);
}

#endif
