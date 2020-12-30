#include "elf.hpp"

#include "debug.hpp"

using namespace ElfHeaders;

void Elf::begin_read_program_header(ElfProgramReader& reader)
{
	reader.header_index = 0;
}

bool Elf::read_next_program_header(ElfProgramReader& reader)
{
	read_header();

	// Check if we reached the end already.
	if (reader.header_index >= m_header.ph_num)
		return false;

	// Seek to the correct location.
	m_reader.seek(Reader::OffsetType::BEGINNING,
			m_header.ph_off + reader.header_index * m_header.ph_entsize);

	m_reader.read(reinterpret_cast<char*>(&reader.header), sizeof(ProgramHeader));

	reader.header_index++;
	return true;
}
