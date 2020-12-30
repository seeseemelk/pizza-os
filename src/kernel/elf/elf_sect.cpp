#include "elf.hpp"

#include "cpu.hpp"
#include "debug.hpp"

using namespace ElfHeaders;

void Elf::begin_read_section_header(ElfSectionReader& reader)
{
	reader.header_index = 0;
}

void Elf::begin_read_section_header(ElfSectionReader& reader, int index)
{
	reader.header_index = index;
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

void Elf::read_section_name(SectionHeader& header, char* dest, size_t len)
{
	if (m_shstrtab_offset == 0)
		find_shstrtab_offset();

	if (m_shstrtab_offset != 0)
	{
		m_reader.seek(Reader::OffsetType::BEGINNING,
				m_shstrtab_offset + header.name);
		m_reader.read(dest, len);
		dest[len - 1] = '\0';
	}
	else
	{
		dest[0] = '\0';
	}
}

void Elf::find_shstrtab_offset()
{
	if (m_header.shstrndx == 0)
		return;

	ElfSectionReader reader;
	begin_read_section_header(reader, m_header.shstrndx);
	read_next_section_header(reader);
	m_shstrtab_offset = reader.header.offset;
}



















