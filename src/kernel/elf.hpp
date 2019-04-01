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
		unsigned short type;
		char machine[2];
		char version_2[4];
		unsigned int entry_point;
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

	enum ElfType
	{
		ET_NONE = 0,
		ET_REL = 1,
		ET_EXEC = 2,
		ET_DYN = 3,
		ET_CORE = 4,
		ET_LOPROC = 0xFF00,
		ET_HIPROC = 0xFFFF
	};

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

	enum ProgramType
	{
		PH_NULL = 0x0,
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

	enum ProgramFlags
	{
		PF_EXECUTE = 0x1,
		PF_WRITE = 0x2,
		PF_READ = 0x4,
		PF_MASKPROC = 0xF0000000
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

	enum SectionType
	{
		SH_NULL = 0x0,
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

	enum SectionFlags
	{
		SHF_WRITE = 0x1, // Should be writable during process execution
		SHF_ALLOC = 0x2, // Occupies memory during process execution
		SHF_EXECINSTR = 0x4, // Contains executable machine instructions
		SHF_MASKPROC = 0xF0000000 // Processor specific semantics
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

	ResultState extract();

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

	ResultState extract_programs();
	ResultState extract_program(ElfHeaders::ProgramHeader& header);

	void extract_sections();
	void extract_section(ElfHeaders::SectionHeader& header);

	void dump_program_headers();
	void dump_section_headers();

	bool is_magic_valid();
	bool are_flags_valid();

	void find_shstrtab_offset();
};

#endif
