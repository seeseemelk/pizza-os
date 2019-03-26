#ifndef ELF_HPP_
#define ELF_HPP_

#include "reader.hpp"

namespace ElfHeaders
{
	enum Endianness
	{
		LITTLE = 1,
		BIG = 2
	};

	struct FileHeader
	{
		char magic_number[4];
		char file_class;
		char endianness;
		char version_1;
		char os_abi;
		char abi_version;
		char pad[7];
		char type[2];
		char machine[2];
		char version_2[4];
		char entry_point[4];
		unsigned int ph_off;
		unsigned int sh_off;
		char flags[4];
		char eh_size[2];
		unsigned short ph_entsize;
		unsigned short ph_num;
		unsigned short sh_entsize;
		unsigned short sh_num;
		unsigned short shstrndx;
	} __attribute__((packed));

	struct ProgramHeader
	{
		unsigned int type;
		unsigned int offset;
		unsigned int vaddr; // Contains virtual address of this segment.
		unsigned int paddr; // Contains physical address of this segment, if relevant.
		unsigned int size_in_file; // Number of bytes of the segment in the image.
		unsigned int size_in_memory; // Number of bytes of the segment in memory.
		unsigned int flags; // Segment-dependent flags.
		unsigned int align; // Alignment. (0 and 1 specify no alignment)
		// End of program header.
		// Note that padding bytes may follow, check ph_entsize.
	};

	enum ProgramHeaderType
	{
		PH_NLL = 0x0, // NULL, but NULL is a reserved symbol.
		PH_LOAD = 0x1,
		PH_DYNAMIC = 0x2,
		PH_INTERP = 0x3,
		PH_NOTE = 0x4,
		PH_SHLIB = 0x5,
		PH_PHDR = 0x6,
		PH_LOOS = 0x60000000,
		PH_HIOS = 0x6FFFFFFF,
		PH_LOPROC = 0x70000000,
		PH_HIPROC = 0x7FFFFFFF
	};

	struct SectionHeader
	{
		unsigned int name; // Offset to a string in .shstrtab
		unsigned int type; // Identifies the type of this header.
		unsigned int flags;
		unsigned int addr; // Virtual address of the section in memory.
		unsigned int offset; // Offset of the section i nthe file image.
		unsigned int size;
		unsigned int link;
		unsigned int info;
		unsigned int addralign; // Alignment of the section. Must be a power of 2.
		unsigned int entsize;
		// End of program header.
		// Note that padding bytes may follow, check sh_entsize.
	};

	enum SectionHeaderType
	{
		SH_NLL = 0x0, // NULL, but NULL is a reserved symbol.
		SH_PROGBITS = 0x1,
		SH_SYMTAB = 0x2,
		SH_STRTAB = 0x3,
		SH_RELA = 0x4,
		SH_HASH = 0x5,
		SH_DYNAMIC = 0x6,
		SH_NOTE = 0x7,
		SH_NOBITS = 0x8,
		SH_REL = 0x9,
		SH_SHLIB = 0xA,
		SH_DYNSYM = 0xB,
		SH_INIT_ARRAY = 0xE,
		SH_FINI_ARRAY = 0xF,
		SH_PREINIT_ARRAY = 0x10,
		SH_GROUP = 0x11,
		SH_SYMTAB_SHNDX = 0x12,
		SH_NUM = 0x13,
		SH_LOOS = 0x60000000 // Start OS-Specific
	};

	struct ElfProgramReader
	{
		int header_index;
		ElfHeaders::ProgramHeader header;
	};

	struct ElfSectionReader
	{
		int header_index;
		ElfHeaders::SectionHeader header;
	};
}

class Elf
{
public:
	Elf(Reader& reader);
	void dump();
	ElfHeaders::FileHeader& read_header();
	bool is_valid();

	void begin_read_program_header(ElfHeaders::ElfProgramReader& reader);
	bool read_next_program_header(ElfHeaders::ElfProgramReader& reader);

	void begin_read_section_header(ElfHeaders::ElfSectionReader& reader);
	void begin_read_section_header(ElfHeaders::ElfSectionReader& reader, int index);
	bool read_next_section_header(ElfHeaders::ElfSectionReader& reader);
	void read_section_name(ElfHeaders::SectionHeader& header, char* dest, size_t len);

private:
	Reader& m_reader;
	ElfHeaders::FileHeader m_header;
	bool m_read_header = false;
	unsigned int m_shstrtab_offset = 0;

	void dump_program_headers();
	void dump_section_headers();

	bool is_magic_valid();
	bool are_flags_valid();

	void find_shstrtab_offset();
};

#endif
