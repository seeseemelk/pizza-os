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
		Process* m_next_process;
		Process* m_previous_process;
		unsigned int m_num_fd = 0;

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

		u32 peek_stack_end(size_t index);
		void poke_stack_end(size_t index, u32 value);

		void open_handle();
		void close_handle();

	private:
		u32 m_page_directory;
		u32 m_syscall_stack[48];
		u32 m_count_handles = 0;
		void* m_esp;

		void load_pagetable();
		void init_stack();
	};

	extern Slab<Process> allocator;
	extern Process* current_process;
	extern u8* process_local_page;

	void init();
	Result<Process*> exec_new_initrd(const char* filename);
	extern unsigned int process_local_index;

	template<typename T> Result<T> allocate_local(size_t bytes)
	{
		if (process_local_index + bytes > KB(4))
			Result<T>();

		T t = static_cast<T>(static_cast<void*>(process_local_page + process_local_index));
		process_local_index += bytes;

		return t;
	}

}

#endif
