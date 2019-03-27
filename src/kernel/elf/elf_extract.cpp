#include "elf.hpp"
#include "pmem.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
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
static ResultState prepare_block(void* address, bool executable, bool writable)
{
	Paging::PageDirEntry& dir_entry = Paging::directory.get_entry(address);

	/*if (!dir_entry.present && dir_entry.make_table().is_fail())
		return ResultState::FAIL;

	dir_entry.userspace = true;
	if (writable)
		dir_entry.writable = true;*/

	Result<Paging::PageTable*> table_result = dir_entry.get_or_make_table();
	if (table_result.is_fail())
		return ResultState::FAIL;

	Paging::PageTableEntry& table_entry = table_result.result->get_entry(address);
	Result<void*> result = PMem::alloc_end(KB(4));
	if (result.is_fail())
		return ResultState::FAIL;

	table_entry.set_address(reinterpret_cast<u32>(result.result));
	table_entry.userspace = true;
	if (writable)
		table_entry.writable = true;
	table_entry.present = 1;

	return ResultState::SUCCESS;
}

/**
 * Extracts a program header and prepares memory for the header.
 */
ResultState Elf::extract_program(ProgramHeader& header)
{
	intptr_t address = header.vaddr;
	size_t num_blocks = ceildiv(header.size_in_memory, KB(4));

	for (size_t i = 0; i < num_blocks; i++)
	{
		if (prepare_block(reinterpret_cast<void*>(address + i * KB(4)),
				header.flags & ProgramFlags::PF_EXECUTE,
				header.flags & ProgramFlags::PF_WRITE) == ResultState::FAIL)
		{
			return ResultState::FAIL;
		}
	}

	log("Copying ELF content to memory...");
	size_t bytes_to_copy = (header.size_in_memory < header.size_in_file) ? header.size_in_memory : header.size_in_file;
	m_reader.seek(Reader::OffsetType::BEGINNING, header.offset);
	m_reader.read(reinterpret_cast<char*>(address), bytes_to_copy);

	return ResultState::SUCCESS;
}

/*void Elf::extract_section(SectionHeader& header)
{
	intptr_t addr = header.addr;
	//header.size
	//return ResultState::FAIL;
}*/
