#include "elf.hpp"
#include "debug.hpp"

using namespace ElfHeaders;

Elf::Elf(Reader& reader)
	: m_reader(reader)
{

}

FileHeader& Elf::read_header()
{
	if (!m_read_header)
	{
		log("Reading file header");
		m_reader.seek(Reader::OffsetType::BEGINNING, 0);
		m_reader.read(reinterpret_cast<char*>(&m_header), sizeof(FileHeader));
		m_read_header = true;
		log("Done");
	}
	return m_header;
}

bool Elf::is_valid()
{
	// Make sure the header has been read.
	read_header();

	// Validate the ELF file.
	return is_magic_valid() && are_flags_valid();
}

bool Elf::is_magic_valid()
{
	return m_header.magic_number[0] == 0x7F
	    && m_header.magic_number[1] == 'E'
	    && m_header.magic_number[2] == 'L'
	    && m_header.magic_number[3] == 'F';
}

bool Elf::are_flags_valid()
{
	return m_header.file_class == 1 // Must be 32-bit
	    && m_header.version_1 == 1 // Must be Current version of ELF
	    && m_header.version_2[0] == 1 && m_header.version_2[1] == 0
	    && m_header.version_2[1] == 0 && m_header.version_2[2] == 0
	    && m_header.endianness == 0x01 // Parser currently only supports little endian ELF headers
	    && m_header.type[0] == 2 && m_header.type[1] == 0 // Must be an executable
	    && m_header.machine[0] == 3 && m_header.machine[1] == 0; // Must be i386
}

void Elf::dump()
{
	dump_program_headers();
	dump_section_headers();
}

void Elf::dump_program_headers()
{
	read_header();
	log("ELF file has %d program headers of %d bytes each starting at 0x%X",
			m_header.ph_num, m_header.ph_entsize, m_header.ph_off);

	ElfProgramReader reader;
	begin_read_program_header(reader);
	while (read_next_program_header(reader))
	{
		if (reader.header.type != ProgramHeaderType::PH_NLL)
		{
			const char* type = "?";
			switch (reader.header.type)
			{
				case ProgramHeaderType::PH_LOAD:
					type = "LOAD";
					break;
			}

			log("Program Header %d", reader.header_index - 1);
			log("  Type: %s (0x%X)", type, reader.header.type);
		}
	}
}

void Elf::dump_section_headers()
{
	read_header();
	log("ELF file has %d section headers of %d bytes each starting at 0x%X",
			m_header.sh_num, m_header.sh_entsize, m_header.sh_off);

	ElfSectionReader reader;
	begin_read_section_header(reader);
	while (read_next_section_header(reader))
	{
		if (reader.header.type != SectionHeaderType::SH_NLL)
		{
			unsigned int alignment = 1 << reader.header.addralign;

			log("Section Header %d", reader.header_index - 1);
			log("  Alignment: 0x%X", alignment);
		}
	}
}




















