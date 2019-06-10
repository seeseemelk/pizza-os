#include "process.hpp"
#include "debug.hpp"
#include "elf.hpp"
#include "ramdisk.hpp"
#include "reader.hpp"
#include "vfs.hpp"

using namespace Proc;

static void link_processes(Process* from, Process* to)
{
	to->m_next_process = from->m_next_process;
	to->m_previous_process = from;
	from->m_next_process = to;
}

void Process::init_stack()
{
	size_t i = sizeof(m_syscall_stack) / sizeof(u32) - 1;
	m_syscall_stack[i--] = 0x23; // User data segment
	m_syscall_stack[i--] = 0;
	m_syscall_stack[i--] = (1 << 9) | (3 << 12); // Flags
	m_syscall_stack[i--] = 0x1B; // User code segment
	m_syscall_stack[i--] = m_entry_point;
	m_esp = m_syscall_stack + i + 1;
	m_syscall_stack[0] = 0xDEADBEEF;
	log("Entry point is at 0x%X", m_entry_point);
}

void Process::init()
{
	log("Initialising Process");
	if (current_process == 0)
	{
		current_process = this;
		m_pid = allocator.index_of(*this) + 1;
		m_uid = 0;
		m_gid = 0;
		CPU::set_ring3_syscall_stack(m_syscall_stack + sizeof(m_syscall_stack) / sizeof(u32));
		m_next_process = this;
		m_previous_process = this;
	}
	else
	{
		m_uid = current_process->m_uid;
		m_gid = current_process->m_gid;
		link_processes(current_process, this);
	}

	log("Performing context switch");
	switch_to();

	log("Setting up program stack");
	init_stack();

	log("Setting up VFS");
	VFS::init_process();

	log("Program loaded");
	open_handle();
}

ResultState Process::validate_stack_protector()
{
	return m_syscall_stack[0] == 0xDEADBEEF ? ResultState::SUCCESS : ResultState::FAIL;
}

ResultState Process::exec_elf(Elf& elf)
{
	init();

	log("Extracting program");
	if (elf.extract() == ResultState::FAIL)
	{
		log("Failed to extract ELF file");
		return ResultState::FAIL;
	}

	return ResultState::SUCCESS;
}

Result<Process*> Proc::exec_new_initrd(const char* filename)
{
	log("Searching for image");
	Result<const RamDisk::TarFile*> tarResult = RamDisk::get_file(filename);
	if (tarResult.is_fail())
	{
		log("Could not find file '%s'", filename);
		return Result<Process*>();
	}
	TarReader reader = TarReader(tarResult.result);

	log("Parsing ELF");
	Elf elf = Elf(reader);

	if (!elf.is_valid())
	{
		log("Invalid ELF file");
		return Result<Process*>();
	}

	log("Allocating process");
	Result<Process*> processResult = Proc::allocator.alloc();
	if (processResult.is_fail())
	{
		log("Could not allocate process");
		return Result<Process*>();
	}

	log("Setting up process");
	Process* process = processResult.result;
	process->m_entry_point = elf.read_header().entry_point;
	process->m_state = ProcessState::STARTING;

	if (process->exec_elf(elf) == ResultState::FAIL)
		return Result<Process*>();
	return Result<Process*>(process);
}

Result<Process*> Proc::exec_empty()
{
	log("Creating new empty process");
	Result<Process*> processResult = Proc::allocator.alloc();
	if (processResult.is_fail())
	{
		log("Failed to create new process");
		return Result<Process*>();
	}

	Process* process = processResult.result;
	process->m_state = ProcessState::STARTING;
	process->init();

	return Result<Process*>(process);
}
























