#include "elf.hpp"

using namespace ElfHeaders;

Elf::Elf(Reader& reader)
	: m_reader(reader)
{

}

FileHeader& Elf::read_header()
{

}
