#include "elf.hpp"
#include "pmem.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include "process.hpp"
#include <cstdint>
#include <cmath>

using namespace ElfHeaders;

/**
 * Extracts the ELF file into the current process' memory.
 */
ResultState Elf::extract()
{
	if (extract_programs() == ResultState::FAIL)
		return ResultState::FAIL;
	return ResultState::SUCCESS;
	/*
	write_sections();
	ElfSectionReader reader;
	begin_read_section_header(reader);

	while (read_next_section_header(reader))
	{
		if (reader.header.type != SectionType::SH_NOBITS && reader.header.flags && SectionFlags::SHF_ALLOC)
		{
			if (extract_section(reader.header) == ResultState::FAIL)
				return ResultState::FAIL;
		}
	}

	return ResultState::SUCCESS;
	*/
}

/**
 * Extracts all program headers in the ELF header into the current process' memory.
 */
ResultState Elf::extract_programs()
{
	log("Creating program segments");
	ElfProgramReader reader;
	begin_read_program_header(reader);

	while (read_next_program_header(reader))
	{
		if (reader.header.type == ProgramType::PH_LOAD)
		{
			log("Creating segment #%d at 0x%X", reader.header_index, reader.header.vaddr);
			if (extract_program(reader.header) == ResultState::FAIL)
				return ResultState::FAIL;
		}
	}
	return ResultState::SUCCESS;
}

/**
 * Allocates a 4 KiB block of memory in userspace.
 */
static ResultState prepare_block(void* address)
{
	return Proc::current_process->map_page(address);
}

static ResultState prepare_blocks(intptr_t address, size_t num_blocks)
{
	for (size_t i = 0; i < num_blocks; i++)
	{
		if (prepare_block(reinterpret_cast<void*>(address + i * KB(4))))
				/*header.flags & ProgramFlags::PF_EXECUTE,
				header.flags & ProgramFlags::PF_WRITE) == ResultState::FAIL)*/
		{
			return ResultState::FAIL;
		}
	}
	return ResultState::SUCCESS;
}

static void constrain_block(void* address, bool writable, bool executable)
{
	// This will be used if the X/D extension is enabled.
	UNUSED(executable);

	Paging::PageDirEntry& dir_entry = Paging::directory.get_entry(address);
	dir_entry.userspace = true;
	if (writable)
		dir_entry.writable = true;

	Paging::PageTableEntry& table_entry = dir_entry.get_table().get_entry(address);
	dir_entry.userspace = true;
	if (writable)
		table_entry.writable = true;
}

static void constrain_blocks(intptr_t address, size_t num_blocks, ProgramHeader& header)
{
	for (size_t i = 0; i < num_blocks; i++)
	{
		constrain_block(reinterpret_cast<void*>(address + i * KB(4)),
				header.flags & ProgramFlags::PF_EXECUTE,
				header.flags & ProgramFlags::PF_WRITE);
	}
}

/**
 * Extracts a program header and prepares memory for the header.
 */
ResultState Elf::extract_program(ProgramHeader& header)
{
	intptr_t address = header.vaddr;
	size_t num_blocks = ceildiv(header.size_in_memory, KB(4));

	if (prepare_blocks(address, num_blocks) == ResultState::FAIL)
		return ResultState::FAIL;


	log("Copying ELF content to memory...");
	size_t bytes_to_copy = (header.size_in_memory < header.size_in_file) ? header.size_in_memory : header.size_in_file;
	m_reader.seek(Reader::OffsetType::BEGINNING, header.offset);
	m_reader.read(reinterpret_cast<char*>(address), bytes_to_copy);

	constrain_blocks(address, num_blocks, header);

	return ResultState::SUCCESS;
}

/*void Elf::extract_section(SectionHeader& header)
{
	intptr_t addr = header.addr;
	//header.size
	//return ResultState::FAIL;
}*/
