#include "process.hpp"
#include "debug.hpp"
#include "elf.hpp"
#include "ramdisk.hpp"
#include "reader.hpp"

using namespace Proc;

ResultState Process::exec_elf(Elf& elf)
{
	if (current_process == 0)
	{
		current_process = this;
		m_pid = static_cast<int>(allocator.index_of(*this));
		m_uid = 0;
		m_gid = 0;
		CPU::set_ring3_syscall_stack(m_syscall_stack);
	}
	else
	{
		m_uid = current_process->m_uid;
		m_gid = current_process->m_gid;
	}

	log("Performing context switch");
	switch_to();
	log("Extracting program");
	if (elf.extract() == ResultState::FAIL)
	{
		log("Failed to extract ELF file");
		return ResultState::FAIL;
	}

	log("Program loaded");
	return ResultState::SUCCESS;
}

Result<Process*> Proc::exec_initrd(const char* filename)
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
