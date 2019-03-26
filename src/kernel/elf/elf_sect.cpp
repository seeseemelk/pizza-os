#include "elf.hpp"
#include "debug.hpp"

using namespace ElfHeaders;

void Elf::begin_read_section_header(ElfSectionReader& reader)
{
	reader.header_index = 0;
}

bool Elf::read_next_section_header(ElfSectionReader& reader)
{
	read_header();

	// Check if we reached the end already.
	if (reader.header_index >= m_header.sh_num)
		return false;

	// Seek to the correct location.
	m_reader.seek(Reader::OffsetType::BEGINNING,
			m_header.sh_off + reader.header_index * m_header.sh_entsize);

	m_reader.read(reinterpret_cast<char*>(&reader.header), sizeof(SectionHeader));

	reader.header_index++;
	return true;
}
