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
		if (reader.header.type != ProgramType::PH_NULL)
		{
			const char* type = "?";
			switch (reader.header.type)
			{
				case ProgramType::PH_LOAD:
					type = "LOAD";
					break;
			}

			log("Program Header %d", reader.header_index - 1);
			log("  Type: %s (0x%X)", type, reader.header.type);
			log("  Address: 0x%X", reader.header.vaddr);
			log("  Size in memory: %d bytes", reader.header.size_in_memory);
			log("  Size in file:   %d bytes", reader.header.size_in_file);
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
		if (reader.header.type != SectionType::SH_NULL)
		{
			char section_name[32];
			read_section_name(reader.header, section_name, sizeof(section_name));

			log("Section Header %d", reader.header_index - 1);
			log("  Name: %s", section_name);
			if (reader.header.addralign > 0)
			{
				unsigned int alignment = 1 << reader.header.addralign;
				log("  Alignment: 0x%X", alignment);
			}

			if (reader.header.addr != 0)
			{
				log("  Memory Location: 0x%X", reader.header.addr);
				log("  Memory size: %d", reader.header.size);
			}

			int bytes_in_file = 0;
			if (reader.header.type != SectionType::SH_NOBITS)
				bytes_in_file = reader.header.size;
			log("  Bytes in file: %d", bytes_in_file);

			if (reader.header.flags != 0)
			{
				log("  Flags:");
				if (reader.header.flags & SectionFlags::SHF_WRITE)
					log("   - WRITE");
				if (reader.header.flags & SectionFlags::SHF_ALLOC)
					log("   - ALLOC");
				if (reader.header.flags & SectionFlags::SHF_EXECINSTR)
					log("   - EXEC");
				if (reader.header.flags & SectionFlags::SHF_MASKPROC)
					log("   - MASKPROC");
			}
		}
	}
}






































