#include <cstddef>
#include <cstring>
#include "debug.hpp"
#include "cpu.hpp"
//#include <vector>

char* vga = reinterpret_cast<char*>(0xB8000);
/*
static void write_string(const char* str)
{
	while (*str != 0)
	{
		*vga = *str;
		str++;
		vga += 2;
	}
}

extern "C" void kernel_main()
{
	std::vector<char> str;
	for (size_t i = 0; i < 10; i++)
		str.push_back('A');

	write_string("Selina is een schatje!");

	for (size_t i = 0; i < str.size(); i++)
		write_string(&str[i]);
}
*/

extern "C" void kernel_main()
{
	vga[0] = 'A';
	Debug::init();
	vga[2] = 'B';
	Debug::put('C');
	vga[4] = 'C';
	Debug::puts("Initialising CPU\n");
	CPU::init();
	CPU::hang();
}
