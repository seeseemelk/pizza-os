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
		char ph_off[4];
		char sh_off[4];
		char flags[4];
		char eh_size[2];
		char ph_entsize[2];
		char ph_num[2];
		char sh_entsize[2];
		char sh_num[2];
		char shstrndx[2];
	};

	struct ProgramHeader
	{
		char type[4];
		char offset[4];
		char vaddr[4];
	};
}

class Elf
{
public:
	Elf(Reader& reader);
	void read_header();
	bool is_valid();

private:
	Reader& m_reader;
};

#endif
