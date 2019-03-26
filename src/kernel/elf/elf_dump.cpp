#include "elf.hpp"
#include "debug.hpp"

using namespace ElfHeaders;

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

			char section_name[32];
			read_section_name(reader.header, section_name, sizeof(section_name));

			log("Section Header %d", reader.header_index - 1);
			log("  Name: %s", section_name);
			log("  Alignment: 0x%X", alignment);
		}
	}
}






































